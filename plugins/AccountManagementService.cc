#include "AccountManagementService.h"

using namespace drogon;

void AccountManagementService::initAndStart(const Json::Value &config) {
    _repo.initSchema();
    LOG_INFO<<"[AccountManagementService] started";
}

void AccountManagementService::shutdown() {
    LOG_INFO<<"[AccountManagementService] shutting down...";
}

Task<ServiceResult<std::vector<AccountEntity>>> AccountManagementService::getAllAccounts() {
    auto data = co_await _repo.getAll();
    co_return ServiceResult<std::vector<AccountEntity>>{true, 0, "getall account Success", data};
}

Task<ServiceResult<AccountEntity>> AccountManagementService::createAccount(const AccountEntity& input) {
    std::string err;
    if (!validate(input, err)) {
        co_return ServiceResult<AccountEntity>{false, 400, err, {}};
    }

    if (co_await _repo.exists(input.username)) {
        co_return ServiceResult<AccountEntity>{false, 400, "Account's username exists", {}};
    }

    if (co_await _repo.insert(input)) {
        co_return ServiceResult<AccountEntity>{true, 0, "Account created", input};
    }
    co_return ServiceResult<AccountEntity>{false, 500, "DB Error", {}};
}

Task<ServiceResult<AccountEntity>> AccountManagementService::getAccountDetail(const std::string& username) {
    auto opt = co_await _repo.getByUsername(username);
    if (!opt) co_return ServiceResult<AccountEntity>{false, 404, "Account Not found", {}};
    co_return ServiceResult<AccountEntity>{true, 0, "get Account detail Success", *opt};
}

Task<ServiceResult<AccountEntity>> AccountManagementService::updateAccount(const AccountEntity& input) {
    if (!co_await _repo.exists(input.username)) {
        co_return ServiceResult<AccountEntity>{false, 404, "Account not found", {}};
    }
    if (co_await _repo.update(input)) {
        co_return ServiceResult<AccountEntity>{true, 0, "Account updated", input};
    }
    co_return ServiceResult<AccountEntity>{false, 500, "Account update failed", {}};
}

Task<ServiceResult<bool>> AccountManagementService::deleteAccount(const std::string& username) {
    if (co_await _repo.remove(username)) {
        co_return ServiceResult<bool>{true, 0, "Account Deleted", true};
    }
    co_return ServiceResult<bool>{false, 404, "Account Not found", false};
}

bool AccountManagementService::validate(const AccountEntity& acc, std::string& outMsg) {
    if (acc.username.empty()) { outMsg = "Account's Username empty"; return false; }
    if (acc.role != "manager" && acc.role != "staff") { outMsg = "Invalid role"; return false; }
    return true;
}