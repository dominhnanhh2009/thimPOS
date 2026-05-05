#pragma once

#include <drogon/drogon.h>
#include <optional>
#include <vector>
#include <string>
#include <cstdint>

/**
 * @struct Product
 * @brief Data model đại diện cho một sản phẩm trong hệ thống POS.
 *
 * @details
 * Struct này ánh xạ trực tiếp với bảng `products` trong database.
 * Không chứa business logic, chỉ là container dữ liệu thuần.
 */
struct Product {
    int64_t id;                 ///< Primary key
    std::string name;           ///< Tên sản phẩm
    int64_t price;              ///< Giá (đơn vị nhỏ nhất, ví dụ: VND)
    std::string description;    ///< Mô tả sản phẩm (có thể rỗng)
    std::string imageUrl;       ///< URL hình ảnh sản phẩm (có thể rỗng)
    std::string category;       ///< Phân loại sản phẩm (vd: drink, food, combo)
    bool isActive;              ///< Trạng thái hoạt động (soft delete / ẩn)
    int64_t createdAt;          ///< Thời điểm tạo (unix timestamp)
    int64_t updatedAt;          ///< Thời điểm cập nhật (unix timestamp)
};

/**
 * @class ProductRepo
 * @brief Repository thao tác trực tiếp với bảng `products` bằng SQL thuần.
 *
 * @details
 * - Không chứa business logic.
 * - Không validate dữ liệu.
 * - Chỉ thực hiện CRUD và mapping DB → struct.
 *
 * @note
 * Designed để dùng với coroutine (`co_await`) của Drogon.
 */
class ProductRepo {
public:
    ProductRepo() = default;

    void init_db();

    drogon::Task<int64_t> createProduct(
        const std::string& name,
        int64_t price,
        const std::string& description,
        const std::string& image_url,
        const std::string& category
    );

    drogon::Task<std::optional<Product>> getById(
        int64_t id,
        bool active_only = false
    );

    drogon::Task<std::vector<Product>> getAll(
        int limit,
        int offset,
        bool active_only = true
    );

    /**
     * @brief Tìm kiếm sản phẩm với filter nâng cao.
     *
     * @param keyword Từ khóa tìm kiếm (LIKE %keyword%)
     * @param category Filter theo category (exact match), nullopt nếu bỏ qua
     * @param from Thời gian bắt đầu (created_at >= from), nullopt nếu bỏ qua
     * @param to Thời gian kết thúc (created_at <= to), nullopt nếu bỏ qua
     * @param limit Số lượng tối đa
     * @param offset Vị trí bắt đầu
     * @param active_only Nếu true chỉ lấy sản phẩm active
     * @return Task danh sách sản phẩm phù hợp
     *
     * @note
     * - Query được build dynamic bằng SQL thuần.
     * - Không dùng ORM.
     * - Các filter là optional → không ảnh hưởng nếu không truyền.
     */
    drogon::Task<std::vector<Product>> search(
        const std::string& keyword,
        const std::optional<std::string>& category,
        const std::optional<int64_t>& from,
        const std::optional<int64_t>& to,
        int limit,
        int offset,
        bool active_only = true
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
    drogon::orm::DbClientPtr getDbClient() const;
};