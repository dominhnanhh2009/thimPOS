#pragma once

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <string>
#include <vector>
#include <optional>

/**
 * @brief Repository xử lý logic truy vấn SQL trực tiếp cho hệ thống Auth.
 */
class AuthNRepo {
public:
    AuthNRepo() = default;

    /**
     * @brief Khởi tạo schema database nếu chưa tồn tại.
     */
    void init();

    /**
     * @brief Xác thực tài khoản và trả về session token nếu hợp lệ.
     * @param username Tên người dùng.
     * @param password Mật khẩu (plaintext hoặc hash tùy implement).
     * @return drogon::Task<std::optional<std::string>> Token phiên làm việc mới nếu login thành công.
     */
    drogon::Task<std::optional<std::string>> login(std::string username, std::string password);

    /**
     * @brief Lấy username từ session token.
     * @param token Token xác thực.
     * @return drogon::Task<std::optional<std::string>> Username tương ứng.
     */
    drogon::Task<std::optional<std::string>> getUserByToken(std::string token);

    /**
     * @brief Thêm user mới vào hệ thống.
     * @return drogon::Task<bool> Trả về true nếu insert thành công.
     */
    drogon::Task<bool> insertUser(std::string username, std::string password);

    /**
     * @brief Xóa user khỏi hệ thống.
     * @return drogon::Task<bool> Trả về true nếu xóa thành công.
     */
    drogon::Task<bool> deleteUser(std::string username);

    /**
     * @brief Cập nhật mật khẩu cho user.
     * @return drogon::Task<bool> Trả về true nếu cập nhật thành công.
     */
    drogon::Task<bool> updatePassword(std::string username, std::string password);

    /**
     * @brief Vô hiệu hóa một session cụ thể.
     * @return drogon::Task<bool> True nếu xóa thành công token.
     */
    drogon::Task<bool> invalidateSession(std::string token);

    /**
     * @brief Vô hiệu hóa tất cả sessions của một user.
     * @return drogon::Task<int> Số lượng token đã bị xóa.
     */
    drogon::Task<int> invalidateAllSessions(std::string username);

    /**
     * @brief Lấy danh sách token hiện có của user.
     * @return drogon::Task<std::vector<std::string>> List các token.
     */
    drogon::Task<std::vector<std::string>> getSessionByUser(std::string username);

    /**
     * @brief Lấy toàn bộ danh sách username trong hệ thống.
     * @return drogon::Task<std::vector<std::string>> Danh sách username.
     */
    drogon::Task<std::vector<std::string>> getAllUsers();
};