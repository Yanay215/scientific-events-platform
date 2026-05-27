#pragma once
#include <drogon/HttpController.h>

class EventController : public drogon::HttpController<EventController> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(EventController::getEvents, "/?tag={tag}&indexed={indexed}&university={university}", drogon::Get);
            METHOD_ADD(EventController::createEvent, "", drogon::Post);
        METHOD_LIST_END
        void getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &tag, const bool &indexed, const std::string &university);
        void createEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};