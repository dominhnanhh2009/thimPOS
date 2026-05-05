#pragma once

#include <drogon/drogon.h>
#include <string>
#include <vector>
#include <optional>

struct AccountEntity {
    std::string username;
    std::string role;
    std::string email;
    std::string phone_number;
    std::string full_name;
    std::string avatar_url;
    std::string date_of_birth;
    std::string created_at;
    std::string updated_at;
};

class AccountRepo {
public:
    void initSchema();
    drogon::Task<std::vector<AccountEntity>> getAll();
    drogon::Task<std::optional<AccountEntity>> getByUsername(const std::string& username);
    drogon::Task<bool> insert(const AccountEntity& account);
    drogon::Task<bool> update(const AccountEntity& account);
    drogon::Task<bool> remove(const std::string& username);
    drogon::Task<bool> exists(const std::string& username);
};