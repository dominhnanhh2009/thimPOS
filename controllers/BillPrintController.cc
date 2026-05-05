#include "BillPrintController.h"
#include "../plugins/OrderService.h"
#include "../plugins/TxtPrinterService.h"

#include <filesystem>
#include <sstream>
#include <drogon/drogon.h>

using namespace drogon;

Task<HttpResponsePtr> BillPrintController::printBill(const HttpRequestPtr req)
{
    Json::Value json;

    // ==========================================================
    // Role Enforcement (Contract: manager, staff)
    // ==========================================================
    auto role = req->getAttributes()->get<std::string>("role");
    if (role != "manager" && role != "staff")
    {
        json["success"] = false;
        json["msg"] = "FORBIDDEN";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k403Forbidden);
        co_return resp;
    }

    try
    {
        // =========================
        // Parse id
        // =========================
        auto idStr = req->getParameter("id");
        if (idStr.empty())
        {
            json["success"] = false;
            json["msg"] = "missing id";
            co_return HttpResponse::newHttpJsonResponse(json);
        }

        int64_t id = std::stoll(idStr);

        // =========================
        // Services
        // =========================
        auto orderService = app().getPlugin<OrderService>();
        auto printerService = app().getPlugin<TxtPrinterService>();

        // =========================
        // Get order
        // =========================
        auto optOrder = co_await orderService->getOrder(id);

        if (!optOrder.has_value())
        {
            json["success"] = false;
            json["msg"] = "ORDER_NOT_FOUND"; // Khớp contract v1.3

            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(k404NotFound);
            co_return resp;
        }

        auto order = optOrder.value();

        // =========================
        // Convert to bill string
        // =========================
        std::string content = orderService->convertOrderToBillString(order);

        // =========================
        // Ensure folder
        // =========================
        std::filesystem::create_directories("bills");

        std::string filePath = "bills/bill_" + std::to_string(id) + ".txt";

        // =========================
        // Save + Print
        // =========================
        co_await printerService->saveStringToFile(content, filePath);
        co_await printerService->printFile(filePath);

        // =========================
        // Success
        // =========================
        json["success"] = true;
        json["msg"] = "PRINT_OK"; // Khớp contract v1.3

        co_return HttpResponse::newHttpJsonResponse(json);
    }
    catch (const std::exception &e)
    {
        json["success"] = false;
        json["msg"] = "PRINT_FAILED"; // Khớp contract v1.3

        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k500InternalServerError);
        co_return resp;
    }
}