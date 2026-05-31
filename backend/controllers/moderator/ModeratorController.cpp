#include "ModeratorController.h"
#include <drogon/orm/DbClient.h>

bool ModeratorController::isModerator(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string role = req->getAttributes()->get<std::string>("user_role");
    if (role != "moderator") {
        Json::Value err;
        err["msg"] = "You are not a moderator";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return false;
    }
    return true;
}

void ModeratorController::getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string sql = "SELECT e.id::text, e.title, e.description, e.start_date, e.end_date, e.created_at "
                      "FROM events e "
                      "WHERE e.status = 'pending'::event_status "
                      "ORDER BY e.created_at ASC";
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
            event["created_at"] = row["created_at"].as<std::string>();
            events.append(event);
        }
        auto resp = drogon::HttpResponse::newHttpJsonResponse(events);
        callback(resp);
    }, [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value err;
        err["msg"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    });
}

void ModeratorController::approveEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!isModerator(req, std::move(callback))) return;
    std::string moderatorId = req->getAttributes()->get<std::string>("user_id");
    std::string eventId = req->getAttributes()->get<std::string>("id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync("UPDATE events SET status = 'approved'::event_status WHERE id = $1::uuid AND status = 'pending'::event_status RETURNING title", [callback, dbClient, moderatorId, eventId](const drogon::orm::Result &r) {
        if (r.empty()) {
            Json::Value err;
            err["msg"] = "Event not found or already processed";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k404NotFound);
            callback(resp);
            return;
        }
        std::string eventTitle = r[0]["title"].as<std::string>();
        Json::Value details;
        details["event_title"] = eventTitle;
        Json::FastWriter writer;
        std::string detailsStr = writer.write(details);
        dbClient->execSqlAsync("INSERT INTO action_logs (moderator_id, action_type, target_type, target_id, details) VALUES ($1::uuid, 'APPROVE_EVENT', 'EVENT', $2::uuid, $3::jsonb)", [callback](const drogon::orm::Result &logResult) {
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = "Event approved successfully";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = "Event approved successfully, but action log failed to be created";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }, moderatorId, eventId, detailsStr);
    }, [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value err;
        err["msg"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }, eventId);
}

void ModeratorController::rejectEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!isModerator(req, std::move(callback))) return;
    auto json = req->getJsonObject();
    if (!json || !(*json)["rejection_reason"].isString()) {
        Json::Value err;
        err["msg"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string eventId = req->getAttributes()->get<std::string>("id");
    std::string moderatorId = req->getAttributes()->get<std::string>("user_id");
    std::string rejectionReason = (*json)["rejection_reason"].asString();
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync("UPDATE events SET status = 'rejected'::event_status, rejection_reason = $1::text WHERE id = $2::uuid AND status = 'pending'::event_status RETURNING title", [callback, dbClient, moderatorId, eventId, rejectionReason](const drogon::orm::Result &r) {
        if (r.empty()) {
            Json::Value err;
            err["msg"] = "Event not found or already processed";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k404NotFound);
            callback(resp);
            return;
        }
        std::string eventTitle = r[0]["title"].as<std::string>();
        Json::Value details;
        details["event_title"] = eventTitle;
        details["rejection_reason"] = rejectionReason;
        Json::FastWriter writer;
        std::string detailsStr = writer.write(details);
        dbClient->execSqlAsync("INSERT INTO action_logs (moderator_id, action_type, target_type, target_id, details) VALUES ($1::uuid, 'REJECT_EVENT', 'EVENT', $2::uuid, $3::jsonb)", [callback](const drogon::orm::Result &logResult) {
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = "Event rejected successfully";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }, [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = "Event rejected successfully, but action log failed to be created";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }, moderatorId, eventId, detailsStr);
    }, [callback](const drogon::orm::DrogonDbException &e) {
        Json::Value err;
        err["msg"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }, rejectionReason, eventId);
}