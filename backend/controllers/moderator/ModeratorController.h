#pragma once
#include <drogon/HttpController.h>

class ModeratorController : public drogon::HttpController<ModeratorController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(ModeratorController::getEvents, "/moderator/events", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(ModeratorController::approveEvent, "/moderator/events/approve", drogon::Post, "JwtFilter");
            ADD_METHOD_TO(ModeratorController::rejectEvent, "/moderator/events/reject", drogon::Post, "JwtFilter");
            ADD_METHOD_TO(ModeratorController::getUsers, "/moderator/users", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(ModeratorController::updateUserRole, "/moderator/users/{id}/role", drogon::Put, "JwtFilter");
            ADD_METHOD_TO(ModeratorController::setUserBlocked, "/moderator/users/{id}/block", drogon::Put, "JwtFilter");
        METHOD_LIST_END
        void getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void approveEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void rejectEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getUsers(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void updateUserRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id);
        void setUserBlocked(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id);
    private:
        bool isModerator(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
