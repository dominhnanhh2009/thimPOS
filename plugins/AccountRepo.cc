#include "AccountRepo.h"

using namespace drogon;

void AccountRepo::initSchema() {
    auto dbClient = app().getDbClient();

    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            username TEXT PRIMARY KEY,
            role TEXT NOT NULL,
            email TEXT,
            phone_number TEXT,
            full_name TEXT,
            avatar_url TEXT,
            date_of_birth TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    dbClient->execSqlSync(sql);
    dbClient->execSqlSync(R"(
        INSERT INTO accounts (username, role, full_name)
        VALUES ('admin', 'manager', 'quản trị viên')
        ON CONFLICT(username) DO NOTHING;
    )");
}

Task<std::vector<AccountEntity>> AccountRepo::getAll() {
    auto result = co_await app().getDbClient()->execSqlCoro("SELECT * FROM accounts");
    std::vector<AccountEntity> list;
    for (const auto& row : result) {
        list.push_back({
            row["username"].as<std::string>(), row["role"].as<std::string>(),
            row["email"].as<std::string>(), row["phone_number"].as<std::string>(),
            row["full_name"].as<std::string>(), row["avatar_url"].as<std::string>(),
            row["date_of_birth"].as<std::string>(), row["created_at"].as<std::string>(),
            row["updated_at"].as<std::string>()
        });
    }
    co_return list;
}

Task<std::optional<AccountEntity>> AccountRepo::getByUsername(const std::string& username) {
    auto result = co_await app().getDbClient()->execSqlCoro("SELECT * FROM accounts WHERE username = $1", username);
    if (result.empty()) co_return std::nullopt;
    auto row = result[0];
    co_return AccountEntity{
        row["username"].as<std::string>(), row["role"].as<std::string>(),
        row["email"].as<std::string>(), row["phone_number"].as<std::string>(),
        row["full_name"].as<std::string>(), row["avatar_url"].as<std::string>(),
        row["date_of_birth"].as<std::string>(), row["created_at"].as<std::string>(),
        row["updated_at"].as<std::string>()
    };
}

Task<bool> AccountRepo::insert(const AccountEntity& a) {
    try {
        co_await app().getDbClient()->execSqlCoro(
            "INSERT INTO accounts (username, role, email, phone_number, full_name, avatar_url, date_of_birth) VALUES ($1, $2, $3, $4, $5, $6, $7)",
            a.username, a.role, a.email, a.phone_number, a.full_name, a.avatar_url, a.date_of_birth);
        co_return true;
    } catch (...) { co_return false; }
}

Task<bool> AccountRepo::update(const AccountEntity& a) {
    auto res = co_await app().getDbClient()->execSqlCoro(
        "UPDATE accounts SET role=$1, email=$2, phone_number=$3, full_name=$4, avatar_url=$5, date_of_birth=$6, updated_at=CURRENT_TIMESTAMP WHERE username=$7",
        a.role, a.email, a.phone_number, a.full_name, a.avatar_url, a.date_of_birth, a.username);
    co_return res.affectedRows() > 0;
}

Task<bool> AccountRepo::remove(const std::string& username) {
    auto res = co_await app().getDbClient()->execSqlCoro("DELETE FROM accounts WHERE username = $1", username);
    co_return res.affectedRows() > 0;
}

Task<bool> AccountRepo::exists(const std::string& username) {
    auto res = co_await app().getDbClient()->execSqlCoro("SELECT 1 FROM accounts WHERE username = $1", username);
    co_return !res.empty();
}