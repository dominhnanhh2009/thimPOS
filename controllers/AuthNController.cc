#include "AuthNController.h"

using namespace drogon;

// --- Helper: Tạo JSON Error theo thống nhất tại mục 5 của contract ---
HttpResponsePtr AuthNController::makeError(const std::string& code, HttpStatusCode status) {
    Json::Value ret;
    ret["success"] = false;
    ret["error"] = code;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(status);
    return resp;
}

// 4.1 Register: POST /auth/register
Task<HttpResponsePtr> AuthNController::registerUser(HttpRequestPtr req) {
    auto json = req->getJsonObject();
    if (!json || !(*json).isMember("username") || !(*json).isMember("password")) {
        co_return makeError("INVALID_INPUT", k400BadRequest);
    }

    std::string user = (*json)["username"].asString();
    std::string pass = (*json)["password"].asString();

    bool ok = co_await service()->insertUser(user, pass);
    if (!ok) {
        co_return makeError("USER_EXISTS", k409Conflict);
    }

    Json::Value ret;
    ret["success"] = true;
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.2 Login: POST /auth/login
Task<HttpResponsePtr> AuthNController::login(HttpRequestPtr req) {
    auto json = req->getJsonObject();
    if (!json || !(*json).isMember("username") || !(*json).isMember("password")) {
        co_return makeError("INVALID_INPUT", k400BadRequest);
    }

    std::string user = (*json)["username"].asString();
    std::string pass = (*json)["password"].asString();

    auto tokenOpt = co_await service()->login(user, pass);
    if (!tokenOpt) {
        co_return makeError("INVALID_CREDENTIALS", k401Unauthorized);
    }

    Json::Value ret;
    ret["success"] = true;
    ret["username"] = user;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    // Cookie sid theo Strategy tại mục 2 của contract
    Cookie cookie("sid", tokenOpt.value());
    cookie.setHttpOnly(true);
    cookie.setPath("/");
    resp->addCookie(cookie);
    
    co_return resp;
}

// 4.3 Logout: POST /auth/logout
Task<HttpResponsePtr> AuthNController::logout(HttpRequestPtr req) {
    auto token = req->getCookie("sid");
    if (!token.empty()) {
        co_await service()->invalidateSession(token);
    }

    Json::Value ret;
    ret["success"] = true;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    
    // Clear cookie
    Cookie cookie("sid", "");
    cookie.setMaxAge(0);
    resp->addCookie(cookie);

    co_return resp;
}

// 4.4 Logout All: POST /auth/logout_all
Task<HttpResponsePtr> AuthNController::logoutAll(HttpRequestPtr req) {
    auto username = req->attributes()->get<std::string>("username");
    if (username.empty()) co_return makeError("UNAUTHORIZED", k401Unauthorized);

    int count = co_await service()->invalidateAllSessions(username);

    Json::Value ret;
    ret["success"] = true;
    ret["invalidated_sessions"] = count;
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.5 Get Current User: GET /auth/me
Task<HttpResponsePtr> AuthNController::getCurrentUser(HttpRequestPtr req) {
    auto username = req->attributes()->get<std::string>("username");
    
    Json::Value ret;
    if (username.empty()) {
        ret["authenticated"] = false;
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k401Unauthorized);
        co_return resp;
    }

    ret["authenticated"] = true;
    ret["username"] = username;
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.6 Get User Sessions: GET /auth/sessions
Task<HttpResponsePtr> AuthNController::getSessions(HttpRequestPtr req) {
    auto username = req->attributes()->get<std::string>("username");
    if (username.empty()) co_return makeError("UNAUTHORIZED", k401Unauthorized);

    auto sessions = co_await service()->getSessionByUser(username);

    Json::Value ret;
    ret["sessions"] = Json::arrayValue;
    for (const auto& s : sessions) ret["sessions"].append(s);
    
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.7 Delete Specific Session: DELETE /auth/sessions/{token}
Task<HttpResponsePtr> AuthNController::deleteSession(HttpRequestPtr req, std::string targetToken) {
    // Xóa session cụ thể dựa trên token từ path
    bool ok = co_await service()->invalidateSession(targetToken);
    
    Json::Value ret;
    ret["success"] = ok;
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.8 Get All Users: GET /auth/users
Task<HttpResponsePtr> AuthNController::getAllUsers(HttpRequestPtr req) {
    // RBAC: Kiểm tra role được gắn bởi AuthZFilter
    auto role = req->attributes()->get<std::string>("role");
    if (role != "manager") {
        co_return makeError("FORBIDDEN", k403Forbidden);
    }

    auto users = co_await service()->getAllUsers();

    Json::Value ret;
    ret["users"] = Json::arrayValue;
    for (const auto& u : users) ret["users"].append(u);
    
    co_return HttpResponse::newHttpJsonResponse(ret);
}

// 4.9 Change Password: POST /auth/change_password
Task<HttpResponsePtr> AuthNController::changePassword(HttpRequestPtr req) {
    // RBAC: Theo contract 4.9 yêu cầu role manager
    auto role = req->attributes()->get<std::string>("role");
    if (role != "manager") {
        co_return makeError("FORBIDDEN", k403Forbidden);
    }

    auto json = req->getJsonObject();
    if (!json || !(*json).isMember("username") || !(*json).isMember("new_password")) {
        co_return makeError("INVALID_INPUT", k400BadRequest);
    }

    std::string user = (*json)["username"].asString();
    std::string newPass = (*json)["new_password"].asString();

    bool ok = co_await service()->updatePassword(user, newPass);

    if (ok) {
        Json::Value ret;
        ret["success"] = true;
        co_return HttpResponse::newHttpJsonResponse(ret);
    } else {
        co_return makeError("USER_NOT_FOUND", k404NotFound);
    }
}

// 4.10 Delete User: DELETE /auth/users/{username}
Task<HttpResponsePtr> AuthNController::deleteUser(HttpRequestPtr req, std::string targetUsername) {
    // 1. RBAC: Kiểm tra quyền từ AuthZFilter gán vào context
    auto role = req->attributes()->get<std::string>("role");
    if (role != "manager") {
        co_return makeError("FORBIDDEN", k403Forbidden);
    }

    if (targetUsername.empty()) {
        co_return makeError("INVALID_INPUT", k400BadRequest);
    }

    // 2. Thực hiện xóa Credentials (và tự động vô hiệu hóa session bên trong service)
    bool ok = co_await service()->deleteUser(targetUsername);

    if (ok) {
        Json::Value ret;
        ret["success"] = true;
        co_return HttpResponse::newHttpJsonResponse(ret);
    } else {
        // 3. Trả lỗi nếu user không tồn tại theo contract
        co_return makeError("USER_NOT_FOUND", k404NotFound);
    }
}