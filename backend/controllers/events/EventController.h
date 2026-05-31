#pragma once
#include <drogon/HttpController.h>

class EventController : public drogon::HttpController<EventController> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(EventController::getEvents, "/?fieldId={fieldId}&indexing={indexing}&university={university}&startDate={startDate}&endDate={endDate}", drogon::Get);
            METHOD_ADD(EventController::createEvent, "", drogon::Post);
        METHOD_LIST_END
        void getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const int &fieldId, const std::string &indexing, const std::string &university, const std::string &startDate, const std::string &endDate);
        void createEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};