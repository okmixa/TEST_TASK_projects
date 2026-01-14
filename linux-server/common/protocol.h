#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <QByteArray>

struct UserData {
    uint32_t userId = 0;
    std::string name;
    std::string email;
};

namespace Protocol {
    constexpr size_t MAX_NAME_LEN = 255;
    constexpr size_t MAX_EMAIL_LEN = 255;

    std::optional<UserData> parse(const QByteArray& buffer);
    QByteArray serialize(const UserData& data);
}

