#pragma once

#include <drogon/HttpController.h>
#include "../plugins/OrderService.h"

using namespace drogon;

class OrderController : public drogon::HttpController<OrderController>
{
public:
    METHOD_LIST_BEGIN
        // Tất cả API Order yêu cầu manager hoặc staff (AuthN -> AuthZ)
        ADD_METHOD_TO(OrderController::createOrder, "/api/orders", Post, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(OrderController::getOrderDetail, "/api/orders/{1}", Get, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(OrderController::listOrders, "/api/orders", Get, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(OrderController::replaceOrder, "/api/orders/{1}", Put, "AuthNFilter", "AuthZFilter");
        ADD_METHOD_TO(OrderController::deleteOrder, "/api/orders/{1}", Delete, "AuthNFilter", "AuthZFilter");
    METHOD_LIST_END

    drogon::Task<drogon::HttpResponsePtr> createOrder(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> getOrderDetail(drogon::HttpRequestPtr req, int64_t id);
    drogon::Task<drogon::HttpResponsePtr> listOrders(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> replaceOrder(drogon::HttpRequestPtr req, int64_t id);
    drogon::Task<drogon::HttpResponsePtr> deleteOrder(drogon::HttpRequestPtr req, int64_t id);

private:
    OrderService* service_ = drogon::app().getPlugin<OrderService>();
};