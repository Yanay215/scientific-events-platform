#include "AuthController.h"
#include <jwt-cpp/jwt.h>
#include "../../services/email/EmailService.h"
#include <random>
#include <cstring>
extern "C" {
    #include <bcrypt.h>
}

const std::string JWT_SECRET = std::getenv("JWT_SECRET") ? std::string(std::getenv("JWT_SECRET")) : "your_default_secret_key";

std::string generateVerificationCode() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    return std::to_string(dis(gen));
}

bool verifyPassword(const std::string &password, const std::string &hash) {
    if (hash.empty() || password.empty()) {
        return false;
    }
    return bcrypt_checkpw(password.c_str(), hash.c_str()) == 0;
}

std::string hashPassword(const std::string &password) {
    char salt[128];
    char hash[128];
    std::memset(salt, 0, sizeof(salt));
    std::memset(hash, 0, sizeof(hash));
    if (bcrypt_gensalt(12, salt) != 0) {
        throw std::runtime_error("Failed to generate salt");
    }
    if (bcrypt_hashpw(password.c_str(), salt, hash) != 0) {
        throw std::runtime_error("Failed to hash password");
    }
    hash[BCRYPT_HASHSIZE - 1] = '\0';
    return std::string(hash);
}

void AuthController::login(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["email"].isString() || !(*json)["password"].isString()) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();

    auto client = drogon::app().getDbClient();
    client->execSqlAsync("SELECT id::text, password_hash, role::text, first_name, last_name, is_verified FROM users WHERE email = $1",
        [password, callback](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value ret;
                ret["result"] = "error";
                ret["error"] = "User not found";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }
            auto row = r[0];
            if (!row["is_verified"].as<bool>()) {
                Json::Value errJson;
                errJson["error"] = "User not verified";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
                resp->setStatusCode(drogon::k403Forbidden);
                callback(resp);
                return;
            }
            std::string password_hash = row["password_hash"].as<std::string>();
            if (!verifyPassword(password, password_hash)) {
                Json::Value ret;
                ret["result"] = "error";
                ret["error"] = "Invalid password";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }

            auto token = jwt::create()
                .set_issuer("urfu_events_app")
                .set_type("JWS")
                .set_payload_claim("user_id", jwt::claim(row["id"].as<std::string>()))
                .set_payload_claim("role", jwt::claim(row["role"].as<std::string>()))
                .sign(jwt::algorithm::hs256{JWT_SECRET});

            Json::Value json;
            json["token"] = token;
            json["user_id"] = row["id"].as<std::string>();
            json["user"]["role"] = row["role"].as<std::string>();
            json["user"]["first_name"] = row["first_name"].as<std::string>();
            json["user"]["last_name"] = row["last_name"].as<std::string>();

            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret;
            ret["result"] = "error";
            ret["error"] = "Internal server error";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        },
        email
    );
}

void AuthController::registerUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["email"].isString() || !(*json)["password"].isString()) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();
    std::string code = generateVerificationCode();
    auto client = drogon::app().getDbClient();
    if (!client) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
        return;
    }
    drogon::app().getDbClient()->execSqlAsync(
        "SELECT id FROM users WHERE email = $1",
        [callback, email, code, password](const drogon::orm::Result &r) {
            if (!r.empty()) {
                Json::Value ret;
                ret["result"] = "error";
                ret["error"] = "User with this email already exists";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k400BadRequest);
                callback(resp);
                return;
            }
            std::string hashed_password = hashPassword(password);
            drogon::app().getDbClient()->execSqlAsync(
                "DELETE FROM user_verifications WHERE email = $1",
                [callback, email, code, hashed_password](const drogon::orm::Result &rDelete) {
                    drogon::app().getDbClient()->execSqlAsync(
                        "INSERT INTO user_verifications (email, password_hash, code, expires_at) "
                        "VALUES ($1, $2, $3, NOW() + INTERVAL '15 minutes') RETURNING id;",
                        [callback, email, code](const drogon::orm::Result &rInsert) {
                            if (rInsert.empty()) {
                                Json::Value ret;
                                ret["result"] = "error";
                                ret["error"] = "Error creating verification session";
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                                resp->setStatusCode(drogon::k400BadRequest);
                                callback(resp);
                                return;
                            }
                            std::string verificationSessionId = rInsert[0]["id"].as<std::string>();
                            EmailService::sendVerificationCode(email, code, [callback, verificationSessionId, email](bool success) {
                                Json::Value jsonRet;
                                jsonRet["result"] = "success";
                                jsonRet["verification_id"] = verificationSessionId; 
                                jsonRet["email"] = email;
                                jsonRet["email_sent"] = success;
                                jsonRet["msg"] = "Verification code sent to email";
                                
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonRet);
                                resp->setStatusCode(drogon::k200OK);
                                callback(resp);
                            });
                        },
                        [callback](const drogon::orm::DrogonDbException &e) {
                            Json::Value ret; ret["error"] = "Error inserting verification session";
                            callback(drogon::HttpResponse::newHttpJsonResponse(ret));
                        },
                        email, hashed_password, code
                    );
                },
                [callback](const drogon::orm::DrogonDbException &e) {
                    Json::Value ret; ret["error"] = "Error clearing old verification sessions";
                    callback(drogon::HttpResponse::newHttpJsonResponse(ret));
                },
                email
            );
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret; ret["error"] = "Error in database while checking email";
            callback(drogon::HttpResponse::newHttpJsonResponse(ret));
        },
        email
    );
}

void AuthController::verifyUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["verification_id"].isString() || !(*json)["code"].isString()) {
        Json::Value ret;
        ret["error"] = "Missing verification_id or code";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string verificationId = (*json)["verification_id"].asString();
    std::string code = (*json)["code"].asString();
    auto client = drogon::app().getDbClient();
    client->execSqlAsync(
        "SELECT email FROM user_verifications WHERE id = $1::uuid AND code = $2 AND expires_at > NOW()",
        [callback, verificationId](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value ret;
                ret["result"] = "error";
                ret["error"] = "Invalid or expired verification code";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k400BadRequest);
                callback(resp);
                return;
            }
            std::string email = r[0]["email"].as<std::string>();
            Json::Value json;
            json["result"] = "success";
            json["verification_id"] = verificationId;
            json["email"] = email;
            json["msg"] = "Verification code verified successfully. Please complete your profile.";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret;
            ret["result"] = "error";
            ret["error"] = "Internal server error";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, verificationId, code);
}

void AuthController::completeProfile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["verification_id"].isString() || 
        !(*json)["first_name"].isString() || !(*json)["last_name"].isString()) {
        Json::Value ret;
        ret["error"] = "Missing required profile fields (First Name, Last Name)";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string verificationId = (*json)["verification_id"].asString();
    std::string firstName = (*json)["first_name"].asString();
    std::string lastName = (*json)["last_name"].asString();
    std::string middleName = (*json)["middle_name"].isString() ? (*json)["middle_name"].asString() : "";
    std::string university = (*json)["university"].isString() ? (*json)["university"].asString() : "";
    std::string department = (*json)["department"].isString() ? (*json)["department"].asString() : "";
    std::string academicDegree = (*json)["academic_degree"].isString() ? (*json)["academic_degree"].asString() : "";
    std::string role = (*json)["role"].isString() ? (*json)["role"].asString() : "participant";
    std::string phone = (*json)["phone"].isString() ? (*json)["phone"].asString() : "";
    std::string birthDate = (*json)["birth_date"].isString() ? (*json)["birth_date"].asString() : "";
    std::string gender = (*json)["gender"].isString() ? (*json)["gender"].asString() : "";

    drogon::app().getDbClient()->execSqlAsync(
        "SELECT email, password_hash FROM user_verifications WHERE id = $1::uuid",
        [callback, verificationId, firstName, lastName, middleName, university, department, academicDegree, role, phone, birthDate, gender](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value ret;
                ret["error"] = "Invalid verification_id";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k400BadRequest);
                callback(resp);
                return;
            }
            std::string email = r[0]["email"].as<std::string>();
            std::string passwordHash = r[0]["password_hash"].as<std::string>();
            drogon::app().getDbClient()->execSqlAsync(
                "INSERT INTO users (email, password_hash, role, first_name, last_name, middle_name, university, department, academic_degree, phone, birth_date, gender, is_verified) "
                "VALUES ($1, $2, $3::user_role, $4, $5, $6, $7, $8, $9, $10, $11, $12, TRUE) RETURNING id",
                [callback, verificationId, email, firstName, lastName, role, phone, birthDate, gender](const drogon::orm::Result &rUser) {
                    std::string newUserId = rUser[0]["id"].as<std::string>();
                    drogon::app().getDbClient()->execSqlAsync("DELETE FROM user_verifications WHERE id = $1::uuid", [](const drogon::orm::Result &){}, [](const drogon::orm::DrogonDbException &){}, verificationId);
                    auto token = jwt::create()
                        .set_issuer("urfu_events_app")
                        .set_type("JWS")
                        .set_payload_claim("user_id", jwt::claim(newUserId))
                        .set_payload_claim("role", jwt::claim(role))
                        .sign(jwt::algorithm::hs256{JWT_SECRET});

                    Json::Value json;
                    json["result"] = "success";
                    json["token"] = token;
                    json["user"]["id"] = newUserId;
                    json["user"]["role"] = role;
                    json["user"]["first_name"] = firstName;
                    json["user"]["last_name"] = lastName;
                    json["user"]["email"] = email;
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
                    resp->setStatusCode(drogon::k200OK);
                    callback(resp);
                },
                [callback](const drogon::orm::DrogonDbException &e) {
                    Json::Value ret;
                    ret["error"] = "Error in database while creating user";
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                    resp->setStatusCode(drogon::k500InternalServerError);
                    callback(resp);
                }, email, passwordHash, role, firstName, lastName, middleName, university, department, academicDegree
                );
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret;
            ret["error"] = "Error in database while fetching user";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, verificationId
        );
}