#pragma once

#include <drogon/plugins/Plugin.h>
#include "AuthNRepo.h"
#include <memory>

/**
 * @brief AuthNService Plugin - Quản lý logic nghiệp vụ Authentication.
 */
class AuthNService : public drogon::Plugin<AuthNService> {
public:
    AuthNService() = default;

    /// Override lifecycle hooks của Plugin
    void initAndStart(const Json::Value &config) override;
    void shutdown() override;

    /**
     * @brief Thực hiện login.
     * @return drogon::Task<std::optional<std::string>> Token mới.
     */
    drogon::Task<std::optional<std::string>> login(std::string username, std::string password);

    /**
     * @brief Lấy thông tin user qua token.
     * @return drogon::Task<std::optional<std::string>> Username.
     */
    drogon::Task<std::optional<std::string>> getUserByToken(std::string token);

    /**
     * @brief Đăng ký user mới.
     */
    drogon::Task<bool> insertUser(std::string username, std::string password);

    /**
     * @brief Xóa user.
     */
    drogon::Task<bool> deleteUser(std::string username);

    /**
     * @brief Đổi mật khẩu.
     */
    drogon::Task<bool> updatePassword(std::string username, std::string password);

    /**
     * @brief Đăng xuất/Vô hiệu hóa 1 session.
     */
    drogon::Task<bool> invalidateSession(std::string token);

    /**
     * @brief Đăng xuất tất cả các thiết bị/phiên.
     */
    drogon::Task<int> invalidateAllSessions(std::string username);

    /**
     * @brief Liệt kê các session của user.
     */
    drogon::Task<std::vector<std::string>> getSessionByUser(std::string username);

    /**
     * @brief Lấy danh sách tất cả username đã đăng ký.
     * @return drogon::Task<std::vector<std::string>> Danh sách username.
     */
    drogon::Task<std::vector<std::string>> getAllUsers();

private:
    std::unique_ptr<AuthNRepo> repo_;
};