#include "../include/protocol.hpp"
#include "parser.h"
#include <cstring>
#include <cctype>
#include <cstdlib>

extern "C" int parse_user_text_c( const char* body,
                                unsigned long body_len,
                                unsigned long long* out_id,
                                char out_name[64],
                                char out_email[64]) {

    if (body_len == 0 || body_len >= 256) return -1;

    char buffer[256];
    std::memcpy(buffer, body, body_len);
    buffer[body_len] = '\0';

    char* saveptr;
    char* token = strtok_r(buffer, " ,\t\r\n", &saveptr);
    const char* tokens[3];
    int count = 0;

    while (token && count < 3) {
        if (std::strlen(token) > 0) {
            tokens[count++] = token;
        }
        token = strtok_r(nullptr, " ,\t\r\n", &saveptr);
    }

    if (count != 3) return -1;

    const char* id_str = nullptr;
    const char* name_str = nullptr;
    const char* email_str = nullptr;

    for (int i = 0; i < 3; ++i) {
        const char* t = tokens[i];
        if (std::strchr(t, '@') != nullptr) {
            if (email_str) return -1;
            email_str = t;
        } else {
            bool is_number = true;
            for (size_t j = 0; t[j] != '\0'; ++j) {
                if (!std::isdigit(static_cast<unsigned char>(t[j]))) {
                    is_number = false;
                    break;
                }
            }
            if (is_number && std::strlen(t) > 0) {
                if (id_str) return -1;
                id_str = t;
            } else {
                if (name_str) return -1;
                name_str = t;
            }
        }
    }

    if (!id_str || !name_str || !email_str) return -1;

    char* end;
    unsigned long long id_val = std::strtoull(id_str, &end, 10);
    if (*end != '\0' || id_val == 0 || id_val > UINT64_MAX) return -1;
    *out_id = id_val;

    size_t name_len = std::strlen(name_str);
    if (name_len == 0 || name_len >= 64) return -1;
    for (size_t i = 0; i < name_len; ++i) {
        unsigned char c = static_cast<unsigned char>(name_str[i]);
        if (c < 32 || c > 126) return -1;
    }
    std::memcpy(out_name, name_str, name_len + 1);

    size_t email_len = std::strlen(email_str);
    if (email_len == 0 || email_len >= 64) return -1;
    if (!std::strchr(email_str, '.')) return -1;
    std::memcpy(out_email, email_str, email_len + 1);

    return 0;
}
