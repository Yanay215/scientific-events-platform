#pragma once
#include <drogon/HttpController.h>

class ModeratorController : public drogon::HttpController<ModeratorController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(ModeratorController::getEvents, "/moderator/events", drogon::Get);
            ADD_METHOD_TO(ModeratorController::approveEvent, "/moderator/events/approve", drogon::Post);
            ADD_METHOD_TO(ModeratorController::rejectEvent, "/moderator/events/reject", drogon::Post);
        METHOD_LIST_END
        void getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void approveEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void rejectEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    private:
        bool isModerator(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};