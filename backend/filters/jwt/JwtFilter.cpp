#include "JwtFilter.h"
#include <jwt-cpp/jwt.h>

extern const std::string JWT_SECRET;

void JwtFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    auto authHeader = req->getHeader("Authorization");
    if (authHeader.empty() || authHeader.find("Bearer ") != 0) {
        auto resp = HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(k401Unauthorized);
        fcb(resp);
        return;
    }
    std::string token = authHeader.substr(7);
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{JWT_SECRET})
            .with_issuer("urfu_events_app");
        verifier.verify(decoded);
        req->getAttributes()["user_id"] = decoded.get_payload_claim("user_id").as_string();
        req->getAttributes()["user_role"] = decoded.get_payload_claim("role").as_string();
        fccb();
    } catch (const std::exception &e) {
        auto resp = HttpResponse::newHttpJsonResponse();
        resp->setStatusCode(k401Unauthorized);
        fcb(resp);
    }
}