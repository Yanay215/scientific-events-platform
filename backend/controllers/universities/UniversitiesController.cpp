#include "UniversitiesController.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>

void UniversitiesController::getAll(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string query = req->getParameter("q");
    std::string safeQuery;
    for (size_t i = 0; i < query.length(); ++i) {
        unsigned char c = query[i];
        if (std::isalnum(c) || c == ' ') {
            safeQuery += c;
        } else if ((c == 0xD0 || c == 0xD1) && (i + 1 < query.length())) {
            unsigned char c2 = query[i + 1];
            if (c2 >= 0x80 && c2 <= 0xBF) {
                safeQuery += c;
                safeQuery += c2;
                ++i;
            }
        }
    }
    std::string command = "python3 " + std::string(std::getenv("SERVICE_PATH")) + "/universities/UniversitiesService.py \"" + safeQuery + "\"";
    std::string result;
    std::array<char, 128> buffer;
    {
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
        if (pipe) {
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
            }
        }
    }
    Json::Value ret;
    Json::Reader reader;
    if (result.empty() || !reader.parse(result, ret)) {
        ret = Json::Value(Json::arrayValue);
    }
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}