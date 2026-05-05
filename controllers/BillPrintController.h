#pragma once

#include <drogon/drogon.h>

using namespace drogon;

class BillPrintController : public drogon::HttpController<BillPrintController>
{
public:
    METHOD_LIST_BEGIN
    // Order v1.4: Yêu cầu cả AuthN (định danh) và AuthZ (phân quyền)
    ADD_METHOD_TO(BillPrintController::printBill, "/api/printbill", Get, "AuthNFilter", "AuthZFilter");
    METHOD_LIST_END

    Task<HttpResponsePtr> printBill(const HttpRequestPtr req);
};