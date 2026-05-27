#pragma once
#include <drogon/HttpController.h>

class ParserController : public drogon::HttpController<ParserController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(ParserController::importParsedEvents, "/parser/import", drogon::Post);
        METHOD_LIST_END
        void importParsedEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    private:
        bool validateApiKey(const drogon::HttpRequestPtr &req);
};