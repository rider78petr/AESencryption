#include "../../include/hmac.hpp"
#include "../../include/sha256.hpp"
#include <cstring>

namespace AES {

void hmac_sha256(const unsigned char* key, size_t keyLen,
                 const unsigned char* data, size_t dataLen,
                 unsigned char digest[HMAC_DIGEST_SIZE]) {
    unsigned char innerPad[64];
    unsigned char outerPad[64];
    unsigned char keyBuf[64] = {0};
    
    if (keyLen > 64) {
        sha256(key, keyLen, keyBuf);
        keyLen = SHA256_DIGEST_SIZE;
    } else {
        memcpy(keyBuf, key, keyLen);
    }
    
    for (int i = 0; i < 64; i++) {
        innerPad[i] = 0x36 ^ keyBuf[i];
        outerPad[i] = 0x5C ^ keyBuf[i];
    }
    
    unsigned char innerHash[SHA256_DIGEST_SIZE];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, innerPad, 64);
    sha256_update(&ctx, data, dataLen);
    sha256_final(&ctx, innerHash);
    
    sha256_init(&ctx);
    sha256_update(&ctx, outerPad, 64);
    sha256_update(&ctx, innerHash, SHA256_DIGEST_SIZE);
    sha256_final(&ctx, digest);
}

}
