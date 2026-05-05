#include "AuthZFilter.h"
#include "../plugins/AccountManagementService.h"

#include <drogon/drogon.h>
#include <json/json.h>

using namespace drogon;

void AuthZFilter::doFilter(const HttpRequestPtr &req,
                           FilterCallback &&fcb,
                           FilterChainCallback &&fccb)
{
    async_run([req,
               fcb = std::move(fcb),
               fccb = std::move(fccb)]() mutable -> Task<void> {

        // 🧠 lấy username (đã được AuthNFilter attach)
        std::string username;
        bool dev_e=false;

        try {
            username = req->attributes()->get<std::string>("username");
        } catch (...) {
            // ⚠️ lỗi dev wiring
            LOG_WARN << "[AuthZFilter] Missing username in request. "
                     << "AuthNFilter might be skipped or misconfigured.";

            Json::Value v;
            v["success"] = false;
            v["error"] = "dev's Config error";

            auto resp = HttpResponse::newHttpJsonResponse(v);
            resp->setStatusCode(k500InternalServerError);

            fcb(resp);
            dev_e=true;
        }
        if(dev_e)co_return;

        // 🚀 gọi AccountManagementService
        auto accountService = app().getPlugin<AccountManagementService>();
        auto result = co_await accountService->getAccountDetail(username);

        // 🔁 forward nguyên lỗi từ service
        if (!result.success) {
            Json::Value v;
            v["success"] = false;
            v["error"] = result.message;

            auto resp = HttpResponse::newHttpJsonResponse(v);

            if (result.errorCode >= 400 && result.errorCode < 600) {
                resp->setStatusCode(
                    static_cast<HttpStatusCode>(result.errorCode));
            } else {
                resp->setStatusCode(k500InternalServerError);
            }

            fcb(resp);
            co_return;
        }

        // 🎯 attach role vào request context
        req->attributes()->insert("role", result.data.role);

        // tiếp tục chain
        fccb();
        co_return;
    });
}