#pragma once

#include <cstdint>
#include <cstddef>

namespace AES {

constexpr size_t SHA256_DIGEST_SIZE = 32;

struct SHA256_CTX {
    uint32_t state[8];
    uint64_t count;
    unsigned char buffer[64];
};

void sha256_init(SHA256_CTX* ctx);

void sha256_update(SHA256_CTX* ctx, const unsigned char* data, size_t len);

void sha256_final(SHA256_CTX* ctx, unsigned char digest[SHA256_DIGEST_SIZE]);

void sha256(const unsigned char* data, size_t len, unsigned char digest[SHA256_DIGEST_SIZE]);

}
