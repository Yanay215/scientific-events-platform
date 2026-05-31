#pragma once
#include <drogon/HttpController.h>

class UniversitiesController : public drogon::HttpController<UniversitiesController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(UniversitiesController::getAll, "/api/universities", drogon::Get);
        METHOD_LIST_END
        void getAll(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};