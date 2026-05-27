#include "FilesController.h"
#include <filesystem>
#include <drogon/utils/Utilities.h>

const std::string FILES_BASE_DIR = "./event_materials";

Json::Value buildDirectoryTree(const std::filesystem::path &path) {
    Json::Value node;
    node["name"] = path.filename().string();
    node["is_directory"] = std::filesystem::is_directory(path);
    node["path"] = std::filesystem::relative(path, FILES_BASE_DIR).string();

    if (std::filesystem::is_directory(path)) {
        Json::Value children(Json::arrayValue);
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            children.append(buildDirectoryTree(entry.path()));
        }
        node["children"] = children;
    }
    return node;
}

void FilesController::uploadFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    drogon::MultiPartParser parser;
    if (parser.parse(req) != 0 || parser.getFiles().empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    const auto &file = parser.getFiles()[0];
    std::filesystem::path raw_name(file.getFileName());
    std::string file_name = raw_name.filename().string();
    if (file_name.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::filesystem::path baseDir(FILES_BASE_DIR);
    if (!std::filesystem::exists(baseDir)) {
        std::filesystem::create_directory(baseDir);
    }
    file.saveTo(baseDir.string());
    std::filesystem::path savedFilePath = baseDir / file.getFileName();
    std::filesystem::path safeSavedFilePath = baseDir / file_name;
    if (savedFilePath != safeSavedFilePath && std::filesystem::exists(savedFilePath)) {
        std::filesystem::rename(savedFilePath, safeSavedFilePath);
    }
    Json::Value ret;
    ret["result"] = "success";
    ret["file_name"] = file_name;
    auto resp = drogon::HttpResponse::newHttpJsonResponse();
    resp->setStatusCode(drogon::k201Created);
    resp->setJsonBody(ret);
    callback(resp);
}

void FilesController::getFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["file_name"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string file_name = (*json)["file_name"].asString();
    if (file_name.find('/') != std::string::npos || file_name.find('\\') != std::string::npos) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string file_path = FILES_BASE_DIR + "/" + file_name;
     if (!std::filesystem::exists(FILES_BASE_DIR)) {
        std::filesystem::create_directory(FILES_BASE_DIR);
    }
    if (!std::filesystem::exists(file_path)) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }
    auto resp = drogon::HttpResponse::newFileResponse(file_path);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}

void FilesController::deleteFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["file_name"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string file_name = (*json)["file_name"].asString();
    if (file_name.find('/') != std::string::npos || file_name.find('\\') != std::string::npos) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string file_path = FILES_BASE_DIR + "/" + file_name;
     if (!std::filesystem::exists(FILES_BASE_DIR)) {
        std::filesystem::create_directory(FILES_BASE_DIR);
    }
    if (!std::filesystem::exists(file_path)) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }

    std::filesystem::remove(file_path);

    auto resp = drogon::HttpResponse::newHttpJsonResponse();
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}

void FilesController::getFiles(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string baseDirPath = FILES_BASE_DIR;
    try {
        if (!std::filesystem::exists(baseDirPath)) {
            std::filesystem::create_directory(baseDirPath);
        }
        Json::Value directoryTree = buildDirectoryTree(baseDirPath);
        auto resp = drogon::HttpResponse::newHttpJsonResponse(directoryTree);
        callback(resp);
    }
    catch (const std::exception &e) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }
}