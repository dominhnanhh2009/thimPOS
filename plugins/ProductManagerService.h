#pragma once

#include <drogon/drogon.h>
#include "ProductRepo.h"

#include <optional>
#include <vector>
#include <string>
#include <cstdint>

/**
 * @class ProductManagerService
 * @brief Service quản lý sản phẩm (business layer).
 *
 * @details
 * - Là drogon plugin (1 plugin = 1 service).
 * - Chứa business logic, validate, orchestration.
 * - Gọi xuống ProductRepo để truy cập DB.
 *
 * @note
 * - Không chứa SQL.
 * - Là entry point cho controller.
 */
class ProductManagerService : public drogon::Plugin<ProductManagerService> {
public:
    ProductManagerService() = default;

    void initAndStart(const Json::Value& config) override;

    void shutdown() override;

    drogon::Task<int64_t> createProduct(
        const std::string& name,
        int64_t price,
        const std::string& description,
        const std::string& image_url,
        const std::string& category
    );

    drogon::Task<std::optional<Product>> getProductById(
        int64_t id,
        bool active_only
    );

    drogon::Task<std::vector<Product>> listProducts(
        int limit,
        int offset,
        bool active_only
    );

    /**
     * @brief Tìm kiếm sản phẩm với filter nâng cao.
     *
     * @param keyword Từ khóa tìm kiếm
     * @param category Filter category (optional)
     * @param from Thời gian bắt đầu (epoch, optional)
     * @param to Thời gian kết thúc (epoch, optional)
     * @param limit Số lượng tối đa
     * @param offset Vị trí bắt đầu
     * @param active_only Nếu true chỉ lấy sản phẩm active
     *
     * @return Task danh sách sản phẩm
     *
     * @note
     * - Service chỉ orchestration, không build SQL.
     * - Forward xuống repo.
     */
    drogon::Task<std::vector<Product>> searchProducts(
        const std::string& keyword,
        const std::optional<std::string>& category,
        const std::optional<int64_t>& from,
        const std::optional<int64_t>& to,
        int limit,
        int offset,
        bool active_only
    );

    drogon::Task<bool> updateProduct(
        int64_t id,
        const std::string& name,
        int64_t price,
        const std::string& description,
        const std::string& image_url,
        const std::string& category,
        bool isActive
    );

    drogon::Task<bool> deleteProduct(int64_t id);

private:
    ProductRepo repo_;
};