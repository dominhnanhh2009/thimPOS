#pragma once

#include <drogon/plugins/Plugin.h>
#include "AccountRepo.h"

/**
 * @struct ServiceResult
 * @brief Wrap kết quả trả về để Controller xử lý HTTP code/JSON
 */
template<typename T>
struct ServiceResult {
    bool success;
    int errorCode; // 0 = success, 400, 404, 500...
    std::string message;
    T data;
};

class AccountManagementService : public drogon::Plugin<AccountManagementService> {
public:
    AccountManagementService() {}

    void initAndStart(const Json::Value &config) override; // Config của Drogon vẫn dùng Json đầu vào
    void shutdown() override;

    // --- Business Logic với Pure C++ Structs ---
    
    drogon::Task<ServiceResult<std::vector<AccountEntity>>> getAllAccounts();
    drogon::Task<ServiceResult<AccountEntity>> createAccount(const AccountEntity& input);
    drogon::Task<ServiceResult<AccountEntity>> getAccountDetail(const std::string& username);
    drogon::Task<ServiceResult<AccountEntity>> updateAccount(const AccountEntity& input);
    drogon::Task<ServiceResult<bool>> deleteAccount(const std::string& username);

private:
    AccountRepo _repo;
    bool validate(const AccountEntity& acc, std::string& outMsg);
};