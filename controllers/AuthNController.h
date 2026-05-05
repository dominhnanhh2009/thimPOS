#pragma once

#include <drogon/HttpController.h>
#include <drogon/utils/FunctionTraits.h>
#include "../plugins/AuthNService.h"
#include "../filters/AuthNFilter.h"
#include "../filters/AuthZFilter.h"

/**
 * @brief AuthNController - Xử lý các endpoint HTTP cho Authentication
 * Tuân thủ contract_authn.md.
 * Sử dụng AuthNFilter để xác thực và AuthZFilter để lấy thông tin Role.
 */
class AuthNController : public drogon::HttpController<AuthNController> {
public:
    METHOD_LIST_BEGIN
        // 1. Public Endpoints (No Filter)
        ADD_METHOD_TO(AuthNController::registerUser, "/auth/register", drogon::Post);
        ADD_METHOD_TO(AuthNController::login, "/auth/login", drogon::Post);
        
        // 2. Manager Endpoints (Yêu cầu xác thực + Role Manager)
        // Theo contract 4.9: change_password yêu cầu role manager
        ADD_METHOD_TO(AuthNController::changePassword, "/auth/change_password", drogon::Post, "AuthNFilter", "AuthZFilter");
        // Theo contract 4.8: get_all_users yêu cầu role manager
        ADD_METHOD_TO(AuthNController::getAllUsers, "/auth/users", drogon::Get, "AuthNFilter", "AuthZFilter");
        // Theo contract 4.10: deleteUser yêu cầu role manager
        ADD_METHOD_TO(AuthNController::deleteUser, "/auth/users/{1}", drogon::Delete, "AuthNFilter", "AuthZFilter");

        // 3. User Endpoints (Yêu cầu xác thực)
        ADD_METHOD_TO(AuthNController::logout, "/auth/logout", drogon::Post, "AuthNFilter");
        ADD_METHOD_TO(AuthNController::logoutAll, "/auth/logout_all", drogon::Post, "AuthNFilter");
        ADD_METHOD_TO(AuthNController::getCurrentUser, "/auth/me", drogon::Get, "AuthNFilter");
        ADD_METHOD_TO(AuthNController::getSessions, "/auth/sessions", drogon::Get, "AuthNFilter");
        ADD_METHOD_TO(AuthNController::deleteSession, "/auth/sessions/{1}", drogon::Delete, "AuthNFilter");
    METHOD_LIST_END

    // Action handlers sử dụng C++20 Coroutines (drogon::Task)
    drogon::Task<drogon::HttpResponsePtr> registerUser(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> login(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> changePassword(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> logout(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> logoutAll(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> getCurrentUser(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> getSessions(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> deleteSession(drogon::HttpRequestPtr req, std::string targetToken);
    drogon::Task<drogon::HttpResponsePtr> getAllUsers(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> deleteUser(drogon::HttpRequestPtr req, std::string targetUsername);

private:
    // Helper tạo error response thống nhất theo mục 5 của contract
    drogon::HttpResponsePtr makeError(const std::string& code, drogon::HttpStatusCode status = drogon::k200OK);
    
    // Truy cập AuthNService plugin (Singleton pattern qua app)
    AuthNService* service() { return drogon::app().getPlugin<AuthNService>(); }
};