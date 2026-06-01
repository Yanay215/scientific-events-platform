#pragma once
#include <drogon/HttpController.h>

class ReportController : public drogon::HttpController<ReportController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(ReportController::exportExcelReport, "/api/reports/participants/excel", drogon::Get, "JwtFilter");
            ADD_METHOD_TO(ReportController::exportWordReport, "/api/reports/summary/word", drogon::Get, "JwtFilter");
        METHOD_LIST_END

        void exportExcelReport(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        void exportWordReport(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    private:
        bool checkStaffRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
