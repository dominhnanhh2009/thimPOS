#include "OrderService.h"

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <vector>

namespace
{

// =======================
// UTF-8 Helpers
// =======================

// Đếm số ký tự hiển thị thực tế (bỏ qua byte tiếp nối trong UTF-8)
int countVisualWidth(const std::string &s) {
    int count = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) count++;
    }
    return count;
}

// Lấy chỉ số byte tại vị trí ký tự thứ N
size_t getByteIndexAtVisualPos(const std::string &s, int n) {
    int count = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (((unsigned char)s[i] & 0xC0) != 0x80) {
            if (count == n) return i;
            count++;
        }
    }
    return s.size();
}

// Hàm pad chuẩn cho UTF-8
std::string padUTF8(const std::string &s, int width) {
    int visualLen = countVisualWidth(s);
    if (visualLen > width) {
        return s.substr(0, getByteIndexAtVisualPos(s, width));
    }
    if (visualLen < width) {
        return s + std::string(width - visualLen, ' ');
    }
    return s;
}

// =======================
// File & String Utils
// =======================

std::string loadFile(const std::string &path)
{
    std::ifstream f(path);
    if (!f.is_open()) return "";
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

struct TokenInfo {
    std::string key;
    int width = -1;
    std::string fmt;
};

TokenInfo parseToken(const std::string &token)
{
    TokenInfo info;
    auto pos = token.find('|');
    if (pos == std::string::npos) {
        info.key = trim(token);
        return info;
    }

    info.key = trim(token.substr(0, pos));
    std::string right = trim(token.substr(pos + 1));

    if (!right.empty() && std::all_of(right.begin(), right.end(), ::isdigit)) {
        info.width = std::stoi(right);
    } else {
        info.fmt = right;
    }
    return info;
}

std::string formatTimestamp(int64_t ts, const std::string &fmt)
{
    std::time_t t = ts;
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    if (fmt == "datetime")
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    else
        oss << std::put_time(&tm, fmt.c_str());
    return oss.str();
}

// =======================
// Core Engine
// =======================

std::string replaceAllTokens(
    const std::string &tpl,
    const std::unordered_map<std::string, std::string> &data)
{
    std::string result;
    result.reserve(tpl.size());

    for (size_t i = 0; i < tpl.size();)
    {
        if (i + 1 < tpl.size() && tpl[i] == '{' && tpl[i + 1] == '{')
        {
            size_t end = tpl.find("}}", i);
            if (end == std::string::npos) break;

            std::string tokenRaw = tpl.substr(i + 2, end - i - 2);
            TokenInfo info = parseToken(tokenRaw);

            auto it = data.find(info.key);
            std::string value = (it != data.end()) ? it->second : "";

            if (!info.fmt.empty() && !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit)) {
                value = formatTimestamp(std::stoll(value), info.fmt);
            }

            if (info.width > 0) {
                value = padUTF8(value, info.width);
            }

            result += value;
            i = end + 2;
        }
        else
        {
            result += tpl[i++];
        }
    }
    return result;
}

bool extractItemsBlock(const std::string &tpl, std::string &before, std::string &block, std::string &after)
{
    const std::string open = "{{#items}}";
    const std::string close = "{{/items}}";
    size_t start = tpl.find(open);
    size_t end = tpl.find(close);

    if (start == std::string::npos || end == std::string::npos || end < start) return false;

    before = tpl.substr(0, start);
    block = tpl.substr(start + open.size(), end - (start + open.size()));
    after = tpl.substr(end + close.size());
    return true;
}

std::string renderItems(const std::vector<OrderItemDTO> &items, const std::string &itemTpl)
{
    std::ostringstream oss;
    for (const auto &it : items)
    {
        std::unordered_map<std::string, std::string> data{
            {"name", it.product_name},
            {"qty", std::to_string(it.quantity)},
            {"price", std::to_string(it.unit_price)},
            {"line_total", std::to_string(it.line_total)},
        };
        oss << replaceAllTokens(itemTpl, data);
    }
    return oss.str();
}

} // namespace

// =======================
// MAIN
// =======================
std::string OrderService::convertOrderToBillString(OrderDTO order)
{
    std::string tpl = loadFile(bill_tpl_filename);
    if (tpl.empty()) return "TEMPLATE NOT FOUND\n";

    std::string before, itemBlock, after;
    bool hasItems = extractItemsBlock(tpl, before, itemBlock, after);

    std::string itemsStr;
    if (hasItems)
        itemsStr = renderItems(order.items, itemBlock);

    std::unordered_map<std::string, std::string> data{
        {"id", std::to_string(order.id)},
        {"created_at", std::to_string(order.created_at)},
        {"updated_at", std::to_string(order.updated_at)},
        {"total_price", std::to_string(order.total_price)},
    };

    if (hasItems)
    {
        return replaceAllTokens(before, data) + itemsStr + replaceAllTokens(after, data);
    }
    else
    {
        return replaceAllTokens(tpl, data);
    }
}