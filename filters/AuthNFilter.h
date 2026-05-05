#pragma once

#include <drogon/HttpFilter.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

/**
 * 🔐 AuthNFilter
 * - Chỉ làm Authentication (không AuthZ)
 * - Resolve session -> username
 * - Attach username vào request attributes
 */
class AuthNFilter : public drogon::HttpFilter<AuthNFilter> {
public:
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};