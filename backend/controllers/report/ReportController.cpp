#include "ReportController.h"
#include <drogon/orm/DbClient.h>
#include <drogon/HttpAppFramework.h>
#include <sstream>
#include <ctime>

static std::string csvEscape(const std::string &value) {
    if (value.find(',') == std::string::npos &&
        value.find('"') == std::string::npos &&
        value.find('\n') == std::string::npos &&
        value.find('\r') == std::string::npos) {
        return value;
    }
    std::string escaped = "\"";
    for (char c : value) {
        if (c == '"') escaped += "\"\"";
        else escaped += c;
    }
    escaped += "\"";
    return escaped;
}

static std::string currentDate() {
    std::time_t now = std::time(nullptr);
    std::tm tmBuf{};
    localtime_r(&now, &tmBuf);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tmBuf);
    return std::string(buf);
}

bool ReportController::checkStaffRole(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string role = req->getAttributes()->get<std::string>("user_role");
    if (role != "organizer" && role != "moderator") {
        Json::Value err;
        err["error"] = "Unauthorized: organizer or moderator role required";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return false;
    }
    return true;
}

void ReportController::exportExcelReport(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!checkStaffRole(req, std::move(callback))) return;
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT a.id::text AS id, u.email, u.phone, "
        "COALESCE(u.university, '') AS university, a.status::text AS status, "
        "e.title AS event_title, a.paper_title "
        "FROM applications a "
        "JOIN users u ON a.user_id = u.id "
        "JOIN events e ON a.event_id = e.id "
        "ORDER BY e.title, u.last_name",
        [callback](const drogon::orm::Result &result) {
            std::stringstream ss;
            ss << "\xEF\xBB\xBF";
            ss << "ID;Email;Телефон;Университет;Статус заявки;Мероприятие;Название доклада\n";
            for (const auto &row : result) {
                ss << csvEscape(row["id"].as<std::string>()) << ";"
                   << csvEscape(row["email"].as<std::string>()) << ";"
                   << csvEscape(row["phone"].as<std::string>()) << ";"
                   << csvEscape(row["university"].as<std::string>()) << ";"
                   << csvEscape(row["status"].as<std::string>()) << ";"
                   << csvEscape(row["event_title"].as<std::string>()) << ";"
                   << csvEscape(row["paper_title"].as<std::string>()) << "\n";
            }
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(ss.str());
            resp->setContentTypeCode(CT_TEXT_CSV);
            resp->addHeader("Content-Disposition", "attachment; filename=participants_report.csv");
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    );
}

void ReportController::exportWordReport(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    if (!checkStaffRole(req, std::move(callback))) return;
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT "
        "(SELECT COUNT(*) FROM events WHERE status = 'approved'::event_status) AS approved_events, "
        "(SELECT COUNT(*) FROM events) AS total_events, "
        "(SELECT COUNT(DISTINCT user_id) FROM applications) AS active_participants, "
        "(SELECT COUNT(*) FROM applications) AS total_applications, "
        "(SELECT COUNT(*) FROM users WHERE role = 'participant'::user_role) AS participant_users",
        [callback](const drogon::orm::Result &result) {
            long approvedEvents = result[0]["approved_events"].as<long>();
            long totalEvents = result[0]["total_events"].as<long>();
            long activeParticipants = result[0]["active_participants"].as<long>();
            long totalApplications = result[0]["total_applications"].as<long>();
            long participantUsers = result[0]["participant_users"].as<long>();

            std::stringstream doc;
            doc << "<html xmlns:o='urn:schemas-microsoft-com:office:office' xmlns:w='urn:schemas-microsoft-com:office:word' xmlns='http://www.w3.org/TR/REC-html40'>\n";
            doc << "<head><meta charset='utf-8'><title>Отчет по научным мероприятиям</title><style>body {font-family: Arial;}</style></head>\n";
            doc << "<body>\n";
            doc << "<h2>АНАЛИТИЧЕСКИЙ ОТЧЕТ ПЛАТФОРМЫ «SCIENTIFIC-EVENTS-PLATFORM»</h2>\n";
            doc << "<p>Дата генерации отчета: " << currentDate() << "</p>\n";
            doc << "<hr/>\n";
            doc << "<p><b>Всего мероприятий в системе:</b> " << totalEvents << "</p>\n";
            doc << "<p><b>Опубликованных (одобренных) конференций:</b> " << approvedEvents << "</p>\n";
            doc << "<p><b>Активных молодых исследователей:</b> " << activeParticipants << "</p>\n";
            doc << "<p><b>Всего поданных заявок:</b> " << totalApplications << "</p>\n";
            doc << "<p><b>Зарегистрированных пользователей с ролью «Участник»:</b> " << participantUsers << "</p>\n";
            doc << "</body></html>";

            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(doc.str());
            resp->setContentTypeCode(CT_TEXT_HTML);
            resp->addHeader("Content-Disposition", "attachment; filename=summary_report.doc");
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e) {
            Json::Value err;
            err["error"] = "Database error: " + std::string(e.base().what());
            auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    );
}
