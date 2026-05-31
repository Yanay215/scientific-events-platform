#include <drogon/drogon.h>
#include <iostream>
#include <cstdlib>
#include <curl/curl.h>

int main(int argc, char* argv[]) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    try {
        drogon::app().loadConfigFile("./config.json");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load configuration: " << e.what() << std::endl;
        return 1;
    }
    drogon::app().registerPreRoutingAdvice([](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::function<void()> &&next) {
        if (req->method() == drogon::HttpMethod::Options) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Parser-Api-Key");
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
            return;
        }
        next();
    });
    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr& req, const drogon::HttpResponsePtr& resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });
    drogon::app().run();
    curl_global_cleanup();
    return 0;
}