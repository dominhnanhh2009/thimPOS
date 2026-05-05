#pragma once

#include <drogon/HttpFilter.h>

/**
 * @brief AuthZFilter
 * 
 * - Đọc username từ request (do AuthNFilter attach)
 * - Gọi AccountManagementService để resolve role
 * - Attach role vào request context
 * 
 * Không xử lý authn, không tự quyết policy
 */
class AuthZFilter : public drogon::HttpFilter<AuthZFilter> {
public:
    AuthZFilter() = default;

    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};