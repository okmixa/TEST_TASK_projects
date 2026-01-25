#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Match the declaration in protocol.hpp
int parse_user_text(
    const char* body,
    unsigned long body_len,
    unsigned long long* out_id,
    char out_name[64],
    char out_email[64]
);

#ifdef __cplusplus
}
#endif

#endif
