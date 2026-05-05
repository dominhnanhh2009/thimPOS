#pragma once
#include <drogon/drogon.h>
#include <string>

using namespace drogon;

/**
 * @brief TxtPrinterService - service cực mỏng xử lý xuất file text và in bill
 *
 * Vai trò:
 *  - Lưu nội dung string ra file .txt
 *  - Gửi file .txt tới hệ thống in (Windows: notepad /pt)
 *
 * Thiết kế:
 *  - Plugin đơn nhiệm (1 plugin = 1 service)
 *  - Coroutine-first (không callback nếu không bắt buộc)
 *  - Không giữ state phức tạp
 *
 * Ghi chú:
 *  - Không ORM
 *  - Không coupling với business layer
 *  - Chỉ xử lý IO thuần
 */
class TxtPrinterService : public drogon::Plugin<TxtPrinterService>
{
public:
    /**
     * @brief Khởi tạo plugin
     *
     * Được gọi tự động bởi Drogon khi server start.
     */
    void initAndStart(const Json::Value &config) override;

    /**
     * @brief Shutdown plugin
     */
    void shutdown() override;

public:
    /**
     * @brief Lưu string vào file .txt
     *
     * @param content Nội dung cần ghi
     * @param filePath Đường dẫn file output (.txt)
     *
     * @return Task<void> coroutine hoàn tất khi ghi xong
     *
     * @note Nếu file chưa tồn tại sẽ tự tạo
     */
    Task<void> saveStringToFile(const std::string &content,
                                const std::string &filePath);

    /**
     * @brief In file .txt bằng hệ thống Windows
     *
     * Thực chất gọi:
     *  notepad /pt <filePath>
     *
     * @param filePath đường dẫn file cần in
     *
     * @return Task<void> hoàn tất khi lệnh print được spawn
     *
     * @warning Chỉ hoạt động trên Windows
     */
    Task<void> printFile(const std::string &filePath);
};