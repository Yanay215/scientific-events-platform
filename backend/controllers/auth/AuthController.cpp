#include "AuthController.h"
#include <jwt-cpp/jwt.h>
#include "bcrypt.h"
#include <random>

extern const std::string JWT_SECRET = "secret";

std::string generateVerificationCode() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    return std::to_string(dis(gen));
}

bool verifyPassword(const std::string &password, const std::string &hash) {
    return bcrypt::validatePassword(password, hash);
}

std::string hashPassword(const std::string &password) {
    return bcrypt::generateHash(password);
}

void AuthController::login(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["email"].isString() || !(*json)["password"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();

    auto client = drogon::app().getDbClient();
    client->execSql("SELECT id::text, password_hash, role::text, first_name, last_name, is_verified FROM users WHERE email = $1",
        [password, callback](const drogon::orm::Result &r) {
            if (r.empty()) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse();
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
                auto resp = drogon::HttpResponse::newHttpJsonResponse();
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
            auto resp = drogon::HttpResponse::newHttpJsonResponse();
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        },
        email
    );
}

void AuthController::registerUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["email"].isString() || !(*json)["password"].isString() || !(*json)["first_name"].isString() || !(*json)["last_name"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();
    std::string first_name = (*json)["first_name"].asString();
    std::string last_name = (*json)["last_name"].asString();
    std::string middle_name = json->get("middle_name", "").asString();
    std::string university = json->get("university", "").asString();
    std::string department = json->get("department", "").asString();
    std::string academic_degree = json->get("academic_degree", "").asString();

    std::string hashed_password = hashPassword(password);

    auto client = drogon::app().getDbClient();
    client->execSql(
        "INSERT INTO users (email, password_hash, first_name, last_name, middle_name, university, department, academic_degree, role) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, 'participant'::user_role) RETURNING id::text, role::text, first_name, last_name",
        [callback](const drogon::orm::Result &r) {
            auto row = r[0];
            std::string userIdStr = row["id"].as<std::string>();
            std::string roleStr = row["role"].as<std::string>();
            std::string code = generateVerificationCode();
            auto token = jwt::create()
                .set_issuer("urfu_events_app")
                .set_type("JWS")
                .set_payload_claim("user_id", jwt::claim(userIdStr))
                .set_payload_claim("role", jwt::claim(roleStr))
                .sign(jwt::algorithm::hs256{JWT_SECRET});
            client->execSql(
                "INSERT INTO user_verifications (user_id, code, expires_at) VALUES ($1::uuid, $2, NOW() + INTERVAL '15 minutes') "
                "ON CONFLICT (user_id) DO UPDATE SET code = $2, expires_at = NOW() + INTERVAL '15 minutes'",
                [callback, email, code, token, userIdStr, row](const drogon::orm::Result &result) {
                    EmailService::sendVerificationCode(email, code, [callback, token, userIdStr, row](bool success) {
                        Json::Value responseJson;
                        responseJson["token"] = token;
                        responseJson["user_id"] = userIdStr;
                        responseJson["user"]["role"] = row["role"].as<std::string>();
                        responseJson["user"]["first_name"] = row["first_name"].as<std::string>();
                        responseJson["user"]["last_name"] = row["last_name"].as<std::string>();
                        responseJson["email_sent"] = success;

                        auto resp = drogon::HttpResponse::newHttpJsonResponse(responseJson);
                        resp->setStatusCode(drogon::k201Created);
                        callback(resp); 
                    });
                }, [callback](const drogon::orm::DrogonDbException &e) {
                    auto resp = drogon::HttpResponse::newHttpJsonResponse();
                    resp->setStatusCode(drogon::k500InternalServerError);
                    callback(resp);
                }, userIdStr, code);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse();
            std::string errorStr = e.base().what();
            if (errorStr.find("23505") != std::string::npos || errorStr.find("duplicate key") != std::string::npos) {
                Json::Value errJson;
                errJson["error"] = "User with this email already exists";
                resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
                resp->setStatusCode(drogon::k400BadRequest);
            } else {
                resp->setStatusCode(drogon::k500InternalServerError);
            }
            callback(resp);
        },
        email, hashed_password, first_name, last_name, middle_name, university, department, academic_degree
    );
}

void AuthController::verifyUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["user_id"].isString() || !(*json)["code"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string userId = (*json)["user_id"].asString();
    std::string code = (*json)["code"].asString();
    auto client = drogon::app().getDbClient();
    client->execSql(
        "SELECT code FROM user_verifications WHERE user_id = $1::uuid AND expires_at > NOW()",
        [callback, client, userId, code](const drogon::orm::Result &result) {
            if (result.empty() || result[0]["code"].as<std::string>() != code) {
                Json::Value errJson;
                errJson["error"] = "Invalid verification code";
                auto resp = drogon::HttpResponse::newHttpJsonResponse();
                resp->setStatusCode(drogon::k400BadRequest);
                callback(resp);
                return;
            }
            client->execSql(
                "BEGIN; "
                "UPDATE users SET is_verified = TRUE WHERE id = $1::uuid; "
                "DELETE FROM user_verifications WHERE user_id = $1::uuid; "
                "COMMIT;",
                [callback](const drogon::orm::Result &r) {
                    Json::Value json;
                    json["result"] = "success";
                    json["msg"] = "User verified successfully";
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
                    resp->setStatusCode(drogon::k200OK);
                    callback(resp);
                }, [callback](const drogon::orm::DrogonDbException &e) {
                    auto resp = drogon::HttpResponse::newHttpJsonResponse();
                    resp->setStatusCode(drogon::k500InternalServerError);
                    callback(resp);
                }, userId);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse();
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, userId); 
}