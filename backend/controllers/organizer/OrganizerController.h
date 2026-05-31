#pragma once
#include <drogon/HttpController.h>

class OrganizerController : public drogon::HttpController<OrganizerController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(OrganizerController::getMyEvents, "/organizer/events", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(OrganizerController::getEventApplications, "/organizer/events/{id}/applications", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(OrganizerController::updateApplicationStatus, "/organizer/applications/{id}/status", drogon::Put, "JwtFilter");
            ADD_METHOD_TO(OrganizerController::exportParticipantsToCsv, "/organizer/events/{id}/export", drogon::Get, "JwtFilter");
        METHOD_LIST_END
        void getMyEvents(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void getEventApplications(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id);
        void updateApplicationStatus(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id);
        void exportParticipantsToCsv(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, const std::string &id);
    private:
        bool checkOrganizerRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};