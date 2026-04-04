#include "AuthController.h"
#include <jwt-cpp/jwt.h>

const std::string JWT_SECRET = "secret";

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
    client->execSql("SELECT id, password_hash, role, first_name, last_name FROM users WHERE email = $1", 
        [password, callback](const drogon::orm::Result &r) {
            if (r.empty()) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse();
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }

            auto row = r[0];
            std::string password_hash = row["password_hash"].as<std::string>();
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

    auto client = drogon::app().getDbClient();
    client->execSql("INSERT INTO users (email, password_hash, first_name, last_name) VALUES ($1, $2, $3, $4) RETURNING id, password_hash, role, first_name, last_name",
        [password, first_name, last_name, callback](const drogon::orm::Result &r) {
            if (r.empty()) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse();
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }

            auto row = r[0];
            std::string password_hash = row["password_hash"].as<std::string>();
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
        email, password, first_name, last_name
    );
}