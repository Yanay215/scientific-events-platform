#include "JwtFilter.h"
#include <jwt-cpp/jwt.h>
#include <drogon/HttpResponse.h>

void JwtFilter::doFilter(const drogon::HttpRequestPtr &req, drogon::FilterCallback &&fcb, drogon::FilterChainCallback &&fccb) {
    auto authHeader = req->getHeader("Authorization");
    if (authHeader.empty() || authHeader.find("Bearer ") != 0) {
        Json::Value err;
        err["error"] = "Invalid authorization header";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
        return;
    }
    std::string token = authHeader.substr(7);
    try {
        auto decoded = jwt::decode(token);
        const char* envSecret = std::getenv("JWT_SECRET");
        std::string jwtSecret = envSecret ? std::string(envSecret) : "your_default_secret_key";
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{jwtSecret})
            .with_issuer("urfu_events_app");
        verifier.verify(decoded);
        req->getAttributes()->insert("user_id", decoded.get_payload_claim("user_id").as_string());
        req->getAttributes()->insert("user_role", decoded.get_payload_claim("role").as_string());
        fccb();
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Invalid or expired token";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
    }
}