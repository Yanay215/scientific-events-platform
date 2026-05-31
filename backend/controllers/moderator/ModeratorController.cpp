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
    if (!isModerator(req, std::move(callback))) return;
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
    auto json = req->getJsonObject();
    if (!json || !(*json)["event_id"].isString()) {
        Json::Value err;
        err["msg"] = "Invalid JSON body: 'event_id' is required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string moderatorId = req->getAttributes()->get<std::string>("user_id");
    std::string eventId = (*json)["event_id"].asString();
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
    if (!json || !(*json)["rejection_reason"].isString() || !(*json)["event_id"].isString()) {
        Json::Value err;
        err["msg"] = "Invalid JSON body: 'event_id' and 'rejection_reason' are required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string eventId = (*json)["event_id"].asString();
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
void ModeratorController::getUsers(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!isModerator(req, std::move(callback))) return;
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT id::text, email, first_name, last_name, middle_name, role::text, is_blocked, created_at "
        "FROM users ORDER BY created_at DESC",
        [callback](const drogon::orm::Result &r) {
            Json::Value users(Json::arrayValue);
            for (const auto &row : r) {
                Json::Value user;
                user["id"] = row["id"].as<std::string>();
                user["email"] = row["email"].as<std::string>();
                user["first_name"] = row["first_name"].as<std::string>();
                user["last_name"] = row["last_name"].as<std::string>();
                user["middle_name"] = row["middle_name"].isNull() ? "" : row["middle_name"].as<std::string>();
                user["role"] = row["role"].as<std::string>();
                user["is_blocked"] = row["is_blocked"].as<bool>();
                user["created_at"] = row["created_at"].as<std::string>();
                users.append(user);
            }
            auto resp = drogon::HttpResponse::newHttpJsonResponse(users);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    );
}

void ModeratorController::updateUserRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id) {
    if (!isModerator(req, std::move(callback))) return;
    auto json = req->getJsonObject();
    if (!json || !(*json)["role"].isString()) {
        Json::Value err;
        err["error"] = "Invalid request body: 'role' is required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string role = (*json)["role"].asString();
    if (role != "participant" && role != "organizer" && role != "moderator") {
        Json::Value err;
        err["error"] = "Invalid role value. Allowed: participant, organizer, moderator";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string moderatorId = req->getAttributes()->get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "UPDATE users SET role = $1::user_role WHERE id = $2::uuid RETURNING email",
        [callback, dbClient, moderatorId, id, role](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value err;
                err["error"] = "User not found";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }
            Json::Value details;
            details["new_role"] = role;
            details["target_email"] = r[0]["email"].as<std::string>();
            Json::FastWriter writer;
            std::string detailsStr = writer.write(details);
            dbClient->execSqlAsync(
                "INSERT INTO action_logs (moderator_id, action_type, target_type, target_id, details) "
                "VALUES ($1::uuid, 'UPDATE_USER_ROLE', 'USER', $2::uuid, $3::jsonb)",
                [](const drogon::orm::Result &) {}, [](const drogon::orm::DrogonDbException &) {},
                moderatorId, id, detailsStr);
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = "User role updated successfully";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, role, id);
}

void ModeratorController::setUserBlocked(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id) {
    if (!isModerator(req, std::move(callback))) return;
    auto json = req->getJsonObject();
    if (!json || !(*json)["is_blocked"].isBool()) {
        Json::Value err;
        err["error"] = "Invalid request body: boolean 'is_blocked' is required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    bool isBlocked = (*json)["is_blocked"].asBool();
    std::string moderatorId = req->getAttributes()->get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "UPDATE users SET is_blocked = $1 WHERE id = $2::uuid RETURNING email",
        [callback, dbClient, moderatorId, id, isBlocked](const drogon::orm::Result &r) {
            if (r.empty()) {
                Json::Value err;
                err["error"] = "User not found";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }
            Json::Value details;
            details["is_blocked"] = isBlocked;
            details["target_email"] = r[0]["email"].as<std::string>();
            Json::FastWriter writer;
            std::string detailsStr = writer.write(details);
            dbClient->execSqlAsync(
                "INSERT INTO action_logs (moderator_id, action_type, target_type, target_id, details) "
                "VALUES ($1::uuid, $2, 'USER', $3::uuid, $4::jsonb)",
                [](const drogon::orm::Result &) {}, [](const drogon::orm::DrogonDbException &) {},
                moderatorId, std::string(isBlocked ? "BLOCK_USER" : "UNBLOCK_USER"), id, detailsStr);
            Json::Value ret;
            ret["result"] = "success";
            ret["msg"] = isBlocked ? "User access restricted" : "User access restored";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, isBlocked, id);
}
