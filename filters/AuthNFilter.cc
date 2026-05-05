#include "AuthNFilter.h"
#include "../plugins/AuthNService.h"
#include <json/json.h>

using namespace drogon;

void AuthNFilter::doFilter(const HttpRequestPtr &req,
                           FilterCallback &&fcb,
                           FilterChainCallback &&fccb)
{
    async_run([req,
               fcb = std::move(fcb),
               fccb = std::move(fccb)]() mutable -> Task<void> {

        auto authService = app().getPlugin<AuthNService>();

        // 🍪 đúng spec: sid
        auto token = req->getCookie("sid");

        auto reject = [&](const std::string &msg) {
            Json::Value v;
            v["success"] = false;
            v["error"] = msg;

            auto resp = HttpResponse::newHttpJsonResponse(v);
            resp->setStatusCode(k401Unauthorized);

            fcb(resp);
        };

        if (token.empty()) {
            reject("UNAUTHORIZED");
            co_return;
        }

        auto usernameOpt = co_await authService->getUserByToken(token);

        if (!usernameOpt.has_value()) {
            reject("UNAUTHORIZED");
            co_return;
        }

        // 🧠 attach identity vào request context
        req->attributes()->insert("username", usernameOpt.value());

        fccb();
        co_return;
    });
}