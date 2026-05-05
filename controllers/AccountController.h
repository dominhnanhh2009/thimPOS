#pragma once

#include <drogon/HttpController.h>
#include <drogon/utils/Utilities.h>
#include <string>
#include "../plugins/AccountManagementService.h"

using namespace drogon;

class AccountController : public drogon::HttpController<AccountController> {
public:
    METHOD_LIST_BEGIN
        // API Quản lý tài khoản: Filter AuthN gán username, AuthZ gán role
        ADD_METHOD_TO(AccountController::getAllAccounts, "/api/accounts", Get, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(AccountController::createAccount, "/api/accounts", Post, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(AccountController::getDetail, "/api/accounts/{1}", Get, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(AccountController::updateAccount, "/api/accounts/{1}", Put, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(AccountController::deleteAccount, "/api/accounts/{1}", Delete, "AuthNFilter", "AuthZFilter");

        // API Cá nhân
        ADD_METHOD_TO(AccountController::getCurrentUser, "/api/me", Get, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(AccountController::updateCurrentUser, "/api/me", Put, "AuthNFilter", "AuthZFilter");
    METHOD_LIST_END

    Task<HttpResponsePtr> getAllAccounts(HttpRequestPtr req);
    Task<HttpResponsePtr> createAccount(HttpRequestPtr req);
    Task<HttpResponsePtr> getDetail(HttpRequestPtr req, std::string username);
    Task<HttpResponsePtr> updateAccount(HttpRequestPtr req, std::string username);
    Task<HttpResponsePtr> deleteAccount(HttpRequestPtr req, std::string username);

    Task<HttpResponsePtr> getCurrentUser(HttpRequestPtr req);
    Task<HttpResponsePtr> updateCurrentUser(HttpRequestPtr req);

private:
    Json::Value toJson(const AccountEntity& acc);
    AccountEntity fromJson(const Json::Value& json);
    HttpResponsePtr makeErrorResponse(int statusCode, const std::string& message);
};