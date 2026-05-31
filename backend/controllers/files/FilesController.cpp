#include "FilesController.h"
#include <filesystem>
#include <fstream>
#include <ctime>
#include <mutex>
#include <drogon/utils/Utilities.h>

const std::string FILES_BASE_DIR = "./event_materials";
const std::string LOG_FILE_PATH = "./application.log";

void logUserAction(const std::string &action, const std::string &status, const std::string &details = "") {
    static std::mutex logMutex;
    std::lock_guard<std::mutex> lock(logMutex);

    std::time_t now = std::time(nullptr);
    std::tm tmBuf{};
    localtime_r(&now, &tmBuf);
    char timeStr[32];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmBuf);

    std::ofstream logFile(LOG_FILE_PATH, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << timeStr << "] | " << action << " | " << status;
        if (!details.empty()) {
            logFile << " | " << details;
        }
        logFile << "\n";
    }
}

static bool isSafeFileName(const std::string &name) {
    if (name.empty()) return false;
    if (name.find('/') != std::string::npos || name.find('\\') != std::string::npos) return false;
    if (name == "." || name == "..") return false;
    if (name.find("..") != std::string::npos) return false;
    return true;
}

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
        logUserAction("UPLOAD_FILE", "ERROR", "Empty or invalid multipart body");
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid multipart body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    const auto &file = parser.getFiles()[0];
    std::filesystem::path raw_name(file.getFileName());
    std::string file_name = raw_name.filename().string();
    if (!isSafeFileName(file_name)) {
        logUserAction("UPLOAD_FILE", "ERROR", "Invalid file name: " + std::string(file.getFileName()));
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid file name";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::filesystem::path baseDir(FILES_BASE_DIR);
    if (!std::filesystem::exists(baseDir)) {
        std::filesystem::create_directory(baseDir);
    }
    file.saveAs((baseDir / file_name).string());

    logUserAction("UPLOAD_FILE", "SUCCESS", "File saved: " + file_name);
    Json::Value ret;
    ret["result"] = "success";
    ret["file_name"] = file_name;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(drogon::k201Created);
    callback(resp);
}

void FilesController::getFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["file_name"].isString()) {
        logUserAction("GET_FILE", "ERROR", "Invalid JSON body");
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string file_name = (*json)["file_name"].asString();
    if (!isSafeFileName(file_name)) {
        logUserAction("GET_FILE", "ERROR", "Invalid file name: " + file_name);
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid file name";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string file_path = FILES_BASE_DIR + "/" + file_name;
    if (!std::filesystem::exists(FILES_BASE_DIR)) {
        std::filesystem::create_directory(FILES_BASE_DIR);
    }
    if (!std::filesystem::exists(file_path)) {
        logUserAction("GET_FILE", "ERROR", "File not found: " + file_name);
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "File not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }    
    if (!std::filesystem::is_regular_file(file_path)) {
        logUserAction("GET_FILE", "ERROR", "Not a regular file: " + file_name);
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "File is not a regular file";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    logUserAction("GET_FILE", "SUCCESS", "File served: " + file_name);
    auto resp = drogon::HttpResponse::newFileResponse(file_path);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}

void FilesController::deleteFile(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !(*json)["file_name"].isString()) {
        logUserAction("DELETE_FILE", "ERROR", "Invalid JSON body");
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid JSON body";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string file_name = (*json)["file_name"].asString();
    if (!isSafeFileName(file_name)) {
        logUserAction("DELETE_FILE", "ERROR", "Invalid file name: " + file_name);
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Invalid file name";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string file_path = FILES_BASE_DIR + "/" + file_name;
    if (!std::filesystem::exists(FILES_BASE_DIR)) {
        std::filesystem::create_directory(FILES_BASE_DIR);
    }
    if (!std::filesystem::exists(file_path)) {
        logUserAction("DELETE_FILE", "ERROR", "File not found: " + file_name);
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "File not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }
    std::filesystem::remove(file_path);
    logUserAction("DELETE_FILE", "SUCCESS", "File deleted: " + file_name);
    Json::Value ret;
    ret["result"] = "success";
    ret["file_name"] = file_name;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
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
        logUserAction("LIST_FILES", "SUCCESS", "Directory tree built");
        auto resp = drogon::HttpResponse::newHttpJsonResponse(directoryTree);
        callback(resp);
    }
    catch (const std::exception &e) {
        logUserAction("LIST_FILES", "ERROR", std::string(e.what()));
        Json::Value ret;
        ret["result"] = "error";
        ret["error"] = "Internal server error";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }
}
