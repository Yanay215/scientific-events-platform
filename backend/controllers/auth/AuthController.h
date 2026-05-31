#pragma once
#include <drogon/HttpController.h>

class AuthController : public drogon::HttpController<AuthController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(AuthController::login, "/api/auth/login", drogon::Post);
            ADD_METHOD_TO(AuthController::registerUser, "/api/auth/register", drogon::Post);
            ADD_METHOD_TO(AuthController::verifyUser, "/api/auth/verify", drogon::Post);
            ADD_METHOD_TO(AuthController::completeProfile, "/api/auth/complete", drogon::Post);
        METHOD_LIST_END

        void login(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void registerUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void verifyUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void completeProfile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};