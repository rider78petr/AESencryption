#pragma once

#include <cstddef>

namespace AES {

constexpr size_t HMAC_DIGEST_SIZE = 32;

void hmac_sha256(const unsigned char* key, size_t keyLen,
                 const unsigned char* data, size_t dataLen,
                 unsigned char digest[HMAC_DIGEST_SIZE]);

}
