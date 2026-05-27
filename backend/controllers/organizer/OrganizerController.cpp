#include "OrganizerController.h"
#include <drogon/orm/DbClient.h>
#include <sstream>

bool OrganizerController::checkOrganizerRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string role = req->getAttributes().get<std::string>("user_role");
    if (role != "organizer" && role != "moderator") {
        Json::Value err;
        err["error"] = "Unauthorized: Organizer role required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return false;
    }
    return true;
}

void OrganizerController::getMyEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!checkOrganizerRole(req, callback)) return;
    std::string organizerId = req->getAttributes().get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT id::text, title, description, start_date, end_date, status::text, rejection_reason "
        "FROM events WHERE organizer_id = $1::uuid ORDER BY start_date DESC",
        [callback](const drogon::orm::Result &r) {
            Json::Value events(Json::arrayValue);
            for (const auto &i : r) {
                Json::Value event;
                event["id"] = i["id"].as<std::string>();
                event["title"] = i["title"].as<std::string>();
                event["description"] = i["description"].as<std::string>();
                event["start_date"] = i["start_date"].as<std::string>();
                event["end_date"] = i["end_date"].as<std::string>();
                event["status"] = i["status"].as<std::string>();
                event["rejection_reason"] = i["rejection_reason"].isNull() ? "" : i["rejection_reason"].as<std::string>();
                events.append(event);
            }
            auto resp = drogon::HttpResponse::newHttpJsonResponse(events);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        },
        organizerId
    );
}

void OrganizerController::getEventApplications(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id) {
    if (!checkOrganizerRole(req, callback)) return;
    std::string organizerId = req->getAttributes().get<std::string>("user_id");
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT id FROM events WHERE id = $1::uuid AND organizer_id = $2::uuid",
        [callback, dbClient, id](const drogon::orm::Result &checkResult) {
            if (checkResult.empty()) {
                Json::Value err;
                err["error"] = "Event not found or access denied";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }
            dbClient->execSqlAsync(
                "SELECT a.id::text, a.paper_title, a.file_path, a.status::text, a.created_at, "
                "u.first_name, u.last_name, u.middle_name, u.university "
                "FROM applications a "
                "JOIN users u ON a.user_id = u.id "
                "WHERE a.event_id = $1::uuid ORDER BY a.created_at ASC",
                [callback](const drogon::orm::Result &appResult) {
                    Json::Value applications(Json::arrayValue);
                    for (const auto &i : appResult) {
                        Json::Value app;
                        app["id"] = i["id"].as<std::string>();
                        app["paper_title"] = i["paper_title"].as<std::string>();
                        app["file_path"] = i["file_path"].isNull() ? "" : i["file_path"].as<std::string>();
                        app["status"] = i["status"].as<std::string>();
                        app["created_at"] = i["created_at"].as<std::string>();
                        Json::Value user;
                        user["first_name"] = i["first_name"].as<std::string>();
                        user["last_name"] = i["last_name"].as<std::string>();
                        user["middle_name"] = i["middle_name"].isNull() ? "" : i["middle_name"].as<std::string>();
                        user["university"] = i["university"].as<std::string>();
                        app["user"] = user;
                        applications.append(app);
                    }
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(applications);
                    callback(resp);
                },
                [callback](const drogon::orm::DrogonDbException &e) {
                    Json::Value err;
                    err["error"] = "Application not found: " + std::string(e.what());
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
                    resp->setStatusCode(drogon::k404NotFound);
                    callback(resp);
                },
                id
            );
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    );
}

void OrganizerController::updateApplicationStatus(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id) {
    if (!checkOrganizerRole(req, callback)) return;
    auto json = req->getJsonObject();
    if (!json || (*json)["status"].isString()) {
        Json::Value err;
        err["error"] = "Invalid request body: 'status' field is required and must be a string";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string statusFilter = (*json)["status"].asString();
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "UPDATE applications SET status = $1::application_status WHERE id = $2::uuid",
        [callback](const drogon::orm::Result &r) {
            Json::Value success;
            success["status"] = "success";
            success["message"] = "Application status updated successfully";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(success);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        },
        statusFilter, id);
}

void OrganizerController::exportParticipantsToCsv(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id) {
    if (!checkOrganizerRole(req, callback)) return;
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT u.last_name, u.first_name, u.middle_name, u.university, u.email, a.paper_title "
        "FROM applications a "
        "JOIN users u ON a.user_id = u.id "
        "WHERE a.event_id = $1::uuid AND a.status = 'approved'::application_status",
        [callback](const drogon::orm::Result &appResult) {
            std::stringstream ss;
            ss << "\xEF\xBB\xBF";
            ss << "Фамилия,Имя,Отчество,ВУЗ / Организация,Email,Название статьи\n";
            for (const auto &i : appResult) {
                std::string middleName = i["middle_name"].isNull() ? "" : i["middle_name"].as<std::string>();
                std::string university = i["university"].isNull() ? "" : i["university"].as<std::string>();
                ss << i["last_name"].as<std::string>() << ","
                   << i["first_name"].as<std::string>() << ","
                   << middleName << ","
                   << university << ","
                   << i["email"].as<std::string>() << ","
                   << i["paper_title"].as<std::string>() << "\n";
            }
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(ss.str());
            resp->setStatusCode(drogon::k200OK);
            resp->setContentTypeCode(drogon::CT_TEXT_CSV);
            resp->addHeader("Content-Disposition", "attachment; filename=participants_report.csv");
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }, id
    );
}