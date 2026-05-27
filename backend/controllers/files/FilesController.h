#pragma once
#include <drogon/HttpController.h>

class FilesController : public drogon::HttpController<FilesController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(FilesController::uploadFile, "/api/files/upload", drogon::Post);
            ADD_METHOD_TO(FilesController::getFile, "/api/files/get", drogon::Get);
            ADD_METHOD_TO(FilesController::deleteFile, "/api/files/delete", drogon::Delete);
            ADD_METHOD_TO(FilesController::getFiles, "/api/files/get", drogon::Get);
        METHOD_LIST_END

        void uploadFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void deleteFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getFiles(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};