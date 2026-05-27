#include "ParserController.h"
#include <drogon/orm/DbClient.h>
#include <drogon/utils/Utilities.h>

const std::string PARSER_API_KEY = "parser_api_key";

bool ParserController::validateApiKey(const drogon::HttpRequestPtr &req) {
    auto apiKey = req->getHeader("X-Parser-Api-Key");
    return (!apiKey.empty() && apiKey == PARSER_API_KEY);
}

void ParserController::importParsedEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!validateApiKey(req)) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k401Unauthorized);
        Json::Value err;
        err["msg"] = "Invalid API key";
        resp->setJsonBody(err);
        callback(resp);
        return;
    }
    auto json = req->getJsonObject();
    if (!json || !(*json)["events"].isArray()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        Json::Value err;
        err["msg"] = "Invalid JSON body: 'events' array is required";
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    const Json::Value &events = (*json)["events"];
    if (events.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        Json::Value ret;
        ret["result"] = "success";
        ret["inserted_events"] = 0;
        resp->setStatusCode(drogon::k200OK);
        resp->setJsonBody(ret);
        callback(resp);
        return;
    }
    auto dbClient = drogon::app().getDbClient();
    auto sharedCounter = std::make_shared<std::atomic<int>>(0);
    auto totalEvents = events.size();
    auto processedEvents = std::make_shared<std::atomic<int>>(0);
    auto errorsList = std::make_shared<std::vector<std::string>>();
    for (const auto &event : events) {
        if (!event["title"].isString() || !event["start_date"].isString() || !event["end_date"].isString() || !event["source_url"].isString()) {
            (*processedEvents)++;
            continue;
        }
        std::string title = event["title"].asString();
        std::string startDate = event["start_date"].asString();
        std::string endDate = event["end_date"].asString();
        std::string sourceUrl = event["source_url"].asString();
        std::string description = event["description"].isString() ? event["description"].asString() : "";
        std::string location = event["location"].isString() ? event["location"].asString() : "";
        bool isOnline = event["is_online"].isBool() ? event["is_online"].asBool() : false;
        std::string indexing = event["indexing"].isString() ? event["indexing"].asString() : "none";
        std::string fieldCode = event["field_code"].isString() ? event["field_code"].asString() : "";
        dbClient->execSqlAsync(
            "SELECT id FROM scientific_fields WHERE code = $1",
            [dbClient, callback, sharedCounter, totalEvents, processedEvents, errorsList, title, startDate, endDate, sourceUrl, description, location, isOnline, indexing](const drogon::orm::Result &r) {
                int fieldId = 1;
                if (r.empty()) {
                    fieldId = r[0]["id"].as<int>();
                } 
                std::string sql = "INSERT INTO events (title, description, start_date, end_date, source_url, field_id, indexing, location, is_online, status, source) "
                                  "VALUES ($1, $2, $3, $4, $5, $6::indexing_type, $7, $8, $9, approved::event_status, 'parsed'::event_source) "
                                  "ON CONFLICT (source_url) DO NOTHING";
                dbClient->execSqlAsync(sql, [sharedCounter, totalEvents, processedEvents, errorsList, callback](const drogon::orm::Result &insertResult) {
                    if (insertResult.affectedRows() > 0) {
                        (*sharedCounter)++;
                    }
                    (*processedEvents)++;
                    if (*processedEvents == totalEvents) {
                        auto resp = drogon::HttpResponse::newHttpJsonResponse();
                        Json::Value ret;
                        ret["result"] = "success";
                        ret["inserted_events"] = sharedCounter->load();  
                        resp->setStatusCode(drogon::k200OK);
                        resp->setJsonBody(ret);
                        callback(resp);
                    }
                }, [totalEvents, processedEvents, errorsList, callback](const drogon::orm::DrogonDbException &e) {
                    (*processedEvents)++;
                    if (*processedEvents == totalEvents) {
                        auto resp = drogon::HttpResponse::newHttpJsonResponse();
                        Json::Value ret;
                        ret["result"] = "partial_success or failure";
                        ret["error"] = e.base().what();
                        resp->setStatusCode(drogon::k500InternalServerError);
                        resp->setJsonBody(ret);
                        callback(resp);
                    }
                }, title, startDate, endDate, sourceUrl, description, location, isOnline, indexing);
            }, [totalEvents, processedEvents, callback](const drogon::orm::DrogonDbException &e) {
                (*processedEvents)++;
                if (*processedEvents == totalEvents) {
                    auto resp = drogon::HttpResponse::newHttpJsonResponse();
                    resp->setStatusCode(drogon::k500InternalServerError);
                    callback(resp);
                }
        }, fieldCode);
    }
}