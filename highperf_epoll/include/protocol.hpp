#pragma once

#include <cstdint>
#include <cstddef>

// --- Message Framing ---
struct MessageHeader {
    uint32_t magic;      // Network byte order (0xCAFEBABE)
    uint32_t length;     // Body size in bytes (network byte order)
    uint16_t type;       // Message type (network byte order)
    uint16_t reserved;   // Padding
} __attribute__((packed));

static constexpr uint32_t PROTOCOL_MAGIC = 0xCAFEBABE;
static constexpr size_t MAX_MESSAGE_BODY_SIZE = 256;

enum MessageType : uint16_t {
    MSG_USER_TEXT = 300,
};

// --- User Record Fields (Text Protocol) ---
// Each message body is a space-separated list of:
//   name=<string> id=<uint64> email=<string>
//
// Example: "name=Alice id=12345 email=alice@example.com"
//
// Field specifications:
static constexpr const char* USER_FIELD_ID    = "id";
static constexpr const char* USER_FIELD_NAME  = "name";
static constexpr const char* USER_FIELD_EMAIL = "email";

// Maximum field value lengths (including null terminator)
static constexpr size_t MAX_USER_NAME_LEN  = 64;
static constexpr size_t MAX_USER_EMAIL_LEN = 64;

// C-compatible function signature for parsing
extern "C" int parse_user_text(
    const char* body,
    unsigned long body_len,
    unsigned long long* out_id,
    char out_name[MAX_USER_NAME_LEN],
    char out_email[MAX_USER_EMAIL_LEN]
);
