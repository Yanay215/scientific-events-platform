#include "EventController.h"
#include <drogon/orm/DbClient.h>

void EventController::getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                const int &fieldId, const std::string &indexing, const std::string &university, const std::string &startDate, const std::string &endDate) {
    std::string sql = "SELECT e.id::text, e.title, e.description, e.start_date, e.end_date, "
                      "e.indexing::text, e.location, e.is_online, f.name AS field_name, u.university AS org_university "
                      "FROM events e "
                      "LEFT JOIN scientific_fields f ON e.field_id = f.id "
                      "LEFT JOIN users u ON e.organizer_id = u.id "
                      "WHERE e.status = 'approved'::event_status";
    std::vector<std::string> params;
    int paramCounter = 1;
    if (fieldId > 0) {
        sql += " AND e.field_id = $" + std::to_string(paramCounter++);
        params.push_back(std::to_string(fieldId));
    }
    if (!indexing.empty() && indexing != "none") {
        sql += " AND e.indexing = $" + std::to_string(paramCounter++) + "::indexing_type";
        params.push_back(indexing);
    }
    if (!university.empty()) {
        sql += " AND u.university = $" + std::to_string(paramCounter++);
        params.push_back(university);
    }
    if (!startDate.empty()) {
        sql += " AND e.start_date >= $" + std::to_string(paramCounter++);
        params.push_back(startDate);
    }
    if (!endDate.empty()) {
        sql += " AND e.end_date <= $" + std::to_string(paramCounter++);
        params.push_back(endDate);
    }
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(sql, [callback](const drogon::orm::Result &r) {
        Json::Value events(Json::arrayValue);
        for (const auto &row : r) {
            Json::Value event;
            event["id"] = row["id"].as<std::string>();
            event["title"] = row["title"].as<std::string>();
            event["description"] = row["description"].as<std::string>();
            event["start_date"] = row["start_date"].as<std::string>();
            event["end_date"] = row["end_date"].as<std::string>();
            event["indexing"] = row["indexing"].as<std::string>();
            event["location"] = row["location"].as<std::string>();
            event["is_online"] = row["is_online"].as<bool>();
            event["field_name"] = row["field_name"].isNull() ? "" : row["field_name"].as<std::string>();
            event["university"] = row["org_university"].isNull() ? "" : row["org_university"].as<std::string>();
            events.append(event);
        }
        auto resp = drogon::HttpResponse::newHttpJsonResponse(events);
        callback(resp);
    }, [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }, paramCounter);
}

void EventController::createEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string title = (*json)["title"].asString();
    std::string description = (*json)["description"].asString();
    std::string startDate = (*json)["start_date"].asString();
    std::string endDate = (*json)["end_date"].asString();
    int fieldId = (*json)["field_id"].asInt();
    std::string indexing = (*json)["indexing"].asString();
    std::string location = (*json)["location"].asString();
    bool isOnline = (*json)["is_online"].asBool();
    std::string organizerId = "NULL";
    std::string userId = req->getAttributes()->get<std::string>("user_id");
    bool hasUser = !userId.empty();
    std::string sql;
    if (hasUser) {
        organizerId = req->getAttributes()->get<std::string>("user_id");
        sql = "INSERT INTO events (title, description, start_date, end_date, field_id, indexing, location, is_online, organizer_id, status, source) "
              "VALUES ($1, $2, $3, $4, $5, $6::indexing_type, $7, $8, $9::uuid, 'pending'::event_status, 'manual'::event_source) RETURNING id::text";
    } else {
        sql = "INSERT INTO events (title, description, start_date, end_date, field_id, indexing, location, is_online, status, source) "
              "VALUES ($1, $2, $3, $4, $5, $6::indexing_type, $7, $8, 'pending'::event_status, 'manual'::event_source) RETURNING id::text";
    }
    auto dbClient = drogon::app().getDbClient();
    auto sqlCallback = [callback](const drogon::orm::Result &r) {
        Json::Value ret;
        ret["result"] = "success";
        ret["id"] = r[0]["id"].as<std::string>();
        ret["msg"] = "Event created successfully and sent to moderation";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k201Created);
        callback(resp);
    };
    auto errorCallback = [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    };
    if (hasUser) {
        dbClient->execSqlAsync(sql, sqlCallback, errorCallback, title, description, startDate, endDate, fieldId, indexing, location, isOnline, organizerId);
    } else {
        dbClient->execSqlAsync(sql, sqlCallback, errorCallback, title, description, startDate, endDate, fieldId, indexing, location, isOnline);
    }
}