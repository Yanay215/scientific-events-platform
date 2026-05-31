#pragma once
#include <drogon/HttpFilter.h>
#include <drogon/HttpRequest.h>

class JwtFilter : public drogon::HttpFilter<JwtFilter> {
    public:
        virtual void doFilter(const drogon::HttpRequestPtr &req, drogon::FilterCallback &&fcb, drogon::FilterChainCallback &&fccb) override;
};