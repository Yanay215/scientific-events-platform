#pragma once
#include <drogon/HttpController.h>

class UserController : public drogon::HttpController<UserController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(UserController::getProfile, "/user/profile", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(UserController::registerToEvent, "/user/events/register", drogon::Post, "JwtFilter");
            ADD_METHOD_TO(UserController::cancelRegistration, "/user/events/{id}/cancel", drogon::Delete, "JwtFilter");
            ADD_METHOD_TO(UserController::getEvents, "/user/events", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(UserController::getCertificates, "/user/certificates", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(UserController::uploadCertificate, "/user/certificates/upload", drogon::Post, "JwtFilter");
        METHOD_LIST_END
        void getProfile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void registerToEvent(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void cancelRegistration(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getCertificates(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void uploadCertificate(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};