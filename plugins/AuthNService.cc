#include "AuthNService.h"
#include <drogon/drogon.h>

using namespace drogon;

void AuthNService::initAndStart(const Json::Value &config) {
    repo_ = std::make_unique<AuthNRepo>();
    try {
        repo_->init();
        LOG_INFO << "AuthNService: Repository initialized successfully.";
    } catch (const std::exception &e) {
        LOG_ERROR << "AuthNService: Failed to init repository: " << e.what();
    }
}

void AuthNService::shutdown() {
    LOG_INFO << "AuthNService: Shutting down...";
    repo_.reset();
}

Task<std::optional<std::string>> AuthNService::login(std::string username, std::string password) {
    co_return co_await repo_->login(username, password);
}

Task<std::optional<std::string>> AuthNService::getUserByToken(std::string token) {
    if (token.empty()) {
        co_return std::nullopt;
    }
    co_return co_await repo_->getUserByToken(token);
}

Task<bool> AuthNService::insertUser(std::string username, std::string password) {
    co_return co_await repo_->insertUser(username, password);
}

Task<bool> AuthNService::deleteUser(std::string username) {
    co_return co_await repo_->deleteUser(username);
}

Task<bool> AuthNService::updatePassword(std::string username, std::string password) {
    co_return co_await repo_->updatePassword(username, password);
}

Task<bool> AuthNService::invalidateSession(std::string token) {
    co_return co_await repo_->invalidateSession(token);
}

Task<int> AuthNService::invalidateAllSessions(std::string username) {
    co_return co_await repo_->invalidateAllSessions(username);
}

Task<std::vector<std::string>> AuthNService::getSessionByUser(std::string username) {
    co_return co_await repo_->getSessionByUser(username);
}

Task<std::vector<std::string>> AuthNService::getAllUsers() {
    co_return co_await repo_->getAllUsers();
}