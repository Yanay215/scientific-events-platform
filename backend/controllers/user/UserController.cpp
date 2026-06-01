#include "UserController.h"
#include <drogon/orm/DbClient.h>

void UserController::getProfile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto userId = req->getAttributes()->get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT id::text, first_name, last_name, middle_name, university, email, department, academic_degree "
        "FROM users WHERE id = $1::uuid",
        [callback](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value err;
                err["error"] = "User not found";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }
            const auto &row = r[0];
            Json::Value profile;
            profile["id"] = row["id"].as<std::string>();
            profile["first_name"] = row["first_name"].as<std::string>();
            profile["last_name"] = row["last_name"].as<std::string>();
            profile["middle_name"] = row["middle_name"].isNull() ? "" : row["middle_name"].as<std::string>();
            profile["university"] = row["university"].isNull() ? "" : row["university"].as<std::string>();
            profile["email"] = row["email"].as<std::string>();
            profile["department"] = row["department"].isNull() ? "" : row["department"].as<std::string>();
            profile["academic_degree"] = row["academic_degree"].isNull() ? "" : row["academic_degree"].as<std::string>();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(profile);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, userId);
}

void UserController::registerToEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["event_id"].isString()) {
        Json::Value err;
        err["error"] = "Invalid request body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    std::string eventId = (*json)["event_id"].as<std::string>();
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "INSERT INTO user_registrations (user_id, event_id, status) "
        "VALUES ($1::uuid, $2::uuid, 'registered'::participation_status) "
        "ON CONFLICT (user_id, event_id) DO NOTHING",
        [callback](const drogon::orm::Result &r) {
        Json::Value ret;
        if (r.affectedRows() == 0) {
            ret["result"] = "warning";
            ret["msg"] = "User already registered to this event";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
            return;
        }
        ret["result"] = "success";
        ret["msg"] = "User registered successfully";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k201Created);
        callback(resp);
    }, [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value errJson;
        errJson["error"] = "Database error: " + std::string(e.base().what());
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }, userId, eventId);
}

void UserController::cancelRegistration(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    std::string eventId = req->getAttributes()->get<std::string>("id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "DELETE FROM user_registrations WHERE user_id = $1::uuid AND event_id = $2::uuid",
        [callback](const drogon::orm::Result &r) {
            Json::Value ret;
            if (r.affectedRows() == 0) {
                ret["err"] = "User was not registered to this event";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }
            ret["result"] = "success";
            ret["msg"] = "User cancelled registration successfully";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value errJson;
            errJson["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, userId, eventId);
}

void UserController::getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT e.id::text, e.title, e.start_date, e.end_date, ur.status::text "
        "FROM events e "
        "JOIN user_registrations ur ON e.id = ur.event_id "
        "WHERE ur.user_id = $1::uuid "
        "ORDER BY e.start_date DESC",
        [callback](const drogon::orm::Result &r) {
            Json::Value events(Json::arrayValue);
            for (const auto &row : r) {
                Json::Value event;
                event["id"] = row["id"].as<std::string>();
                event["title"] = row["title"].as<std::string>();
                event["start_date"] = row["start_date"].as<std::string>();
                event["end_date"] = row["end_date"].as<std::string>();
                event["status"] = row["status"].as<std::string>();
                events.append(event);
            }
            auto resp = drogon::HttpResponse::newHttpJsonResponse(events);
            callback(resp);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value errJson;
            errJson["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, userId
    );
}

void UserController::getCertificates(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT c.id, c.title, c.uploaded_at, c.file_name "
        "FROM certificates c "
        "WHERE c.user_id = $1 ",
        [callback](const drogon::orm::Result &r) {
            Json::Value certificates(Json::arrayValue);
            for (const auto &row : r) {
                Json::Value certificate;
                certificate["id"] = row["id"].as<std::string>();
                certificate["title"] = row["title"].as<std::string>();
                certificate["uploaded_at"] = row["uploaded_at"].as<std::string>();
                certificate["file_name"] = row["file_name"].as<std::string>();
                certificates.append(certificate);
            }
            auto resp = drogon::HttpResponse::newHttpJsonResponse(certificates);
            callback(resp);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value errJson;
            errJson["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, userId
    );
}

void UserController::uploadCertificate(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["title"].isString() || !(*json)["file_name"].isString() || !(*json)["file_path"].isString()) {
        Json::Value errJson;
        errJson["error"] = "Invalid request body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string title = (*json)["title"].asString();
    std::string fileName = (*json)["file_name"].asString();
    std::string filePath = (*json)["file_path"].asString();
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    std::string eventId = (*json)["event_id"].isString() ? (*json)["event_id"].asString() : "";
    auto dbClient = drogon::app().getDbClient();
    std::string sql = "INSERT INTO certificates (user_id, title, file_path, file_name";
    if (!eventId.empty()) sql += ", event_id) VALUES ($1::uuid, $2, $3, $4, $5::uuid)";
    else sql += ") VALUES ($1::uuid, $2, $3, $4)";
    auto sqlCallback = [callback](const drogon::orm::Result &r) {
        Json::Value ret;
        ret["result"] = "success";
        ret["msg"] = "Certificate uploaded successfully";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k201Created);
        callback(resp);
    };
    auto errorCallback = [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value errJson;
        errJson["error"] = "Database error: " + std::string(e.base().what());
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    };
    if (!eventId.empty()) {
        dbClient->execSqlAsync(sql, sqlCallback, errorCallback, userId, title, filePath, fileName, eventId);
    } else {
        dbClient->execSqlAsync(sql, sqlCallback, errorCallback, userId, title, filePath, fileName);
    }
}