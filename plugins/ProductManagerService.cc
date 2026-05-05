#include "ProductManagerService.h"
#include <stdexcept>
#include <algorithm>

using namespace drogon;

void ProductManagerService::initAndStart(const Json::Value& config) {
    repo_.init_db();
    LOG_INFO << "[ProductManagerService] started";
}

void ProductManagerService::shutdown() {
    LOG_INFO << "[ProductManagerService] shutdown";
}

Task<int64_t> ProductManagerService::createProduct(const std::string& name, int64_t price, const std::string& description, const std::string& image_url, const std::string& category) {
    if (name.empty()) throw std::invalid_argument("Product name must not be empty");
    if (price < 0) throw std::invalid_argument("Price must be >= 0");
    co_return co_await repo_.createProduct(name, price, description, image_url, category);
}

Task<std::optional<Product>> ProductManagerService::getProductById(int64_t id, bool active_only) {
    if (id <= 0) co_return std::nullopt;
    co_return co_await repo_.getById(id, active_only);
}

Task<std::vector<Product>> ProductManagerService::listProducts(int limit, int offset, bool active_only) {
    limit = std::clamp(limit, 1, 100);
    offset = std::max(0, offset);
    co_return co_await repo_.getAll(limit, offset, active_only);
}

Task<std::vector<Product>> ProductManagerService::searchProducts(
    const std::string& keyword,
    const std::optional<std::string>& category,
    const std::optional<int64_t>& from,
    const std::optional<int64_t>& to,
    int limit,
    int offset,
    bool active_only
) {
    limit = std::clamp(limit, 1, 100);
    offset = std::max(0, offset);
    
    // Logic: Nếu không có keyword và không có filter nào khác, có thể trả về list mặc định hoặc rỗng tùy yêu cầu.
    // Ở đây ta forward thẳng xuống repo để repo xử lý filter.
    co_return co_await repo_.search(keyword, category, from, to, limit, offset, active_only);
}

Task<bool> ProductManagerService::updateProduct(int64_t id, const std::string& name, int64_t price, const std::string& description, const std::string& image_url, const std::string& category, bool isActive) {
    if (id <= 0 || name.empty() || price < 0) co_return false;
    
    auto existing = co_await repo_.getById(id, false);
    if (!existing) co_return false;

    co_return co_await repo_.updateProduct(id, name, price, description, image_url, category, isActive);
}

Task<bool> ProductManagerService::deleteProduct(int64_t id) {
    if (id <= 0) co_return false;
    auto existing = co_await repo_.getById(id, false);
    if (!existing) co_return false;
    co_return co_await repo_.deleteProduct(id);
}