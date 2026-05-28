#include "../../include/pbkdf2.hpp"
#include "../../include/hmac.hpp"
#include <cstring>
#include <random>
#include <stdexcept>

namespace AES {

std::vector<byte> generateSalt(size_t length) {
    std::vector<byte> salt(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    for (size_t i = 0; i < length; ++i) {
        salt[i] = static_cast<byte>(dis(gen));
    }
    return salt;
}

void pbkdf2_hmac_sha256(const byte* password, size_t passwordLen,
                        const byte* salt, size_t saltLen,
                        int iterations, int keyLength,
                        byte* output) {
    if (!salt || saltLen == 0) {
        throw std::invalid_argument("Salt must not be null or empty");
    }
    if (!password || passwordLen == 0) {
        throw std::invalid_argument("Password must not be null or empty");
    }
    if (saltLen > 1024 * 1024) {
        throw std::invalid_argument("Salt too large");
    }

    unsigned char digest[HMAC_DIGEST_SIZE];
    unsigned char counter[4] = {0, 0, 0, 1};

    int blocks = (keyLength + (int)HMAC_DIGEST_SIZE - 1) / (int)HMAC_DIGEST_SIZE;

    for (int block = 1; block <= blocks; block++) {
        counter[3] = block & 0xFF;
        counter[2] = (block >> 8) & 0xFF;
        counter[1] = (block >> 16) & 0xFF;
        counter[0] = (block >> 24) & 0xFF;

        std::vector<byte> message(saltLen + 4);
        memcpy(message.data(), salt, static_cast<size_t>(saltLen));
        memcpy(message.data() + saltLen, counter, 4);

        hmac_sha256(password, passwordLen,
                    message.data(), message.size(),
                    digest);

        size_t offset = (size_t)(block - 1) * HMAC_DIGEST_SIZE;
        size_t bytesToCopy = HMAC_DIGEST_SIZE;
        if (offset + bytesToCopy > (size_t)keyLength) {
            bytesToCopy = (size_t)keyLength - offset;
        }
        memcpy(output + offset, digest, bytesToCopy);

        unsigned char prev[HMAC_DIGEST_SIZE];
        memcpy(prev, digest, HMAC_DIGEST_SIZE);

        for (int iter = 1; iter < iterations; iter++) {
            hmac_sha256(password, passwordLen, prev, HMAC_DIGEST_SIZE, digest);
            for (size_t i = 0; i < bytesToCopy; i++) {
                output[offset + i] ^= digest[i];
            }
            memcpy(prev, digest, HMAC_DIGEST_SIZE);
        }
    }
}

std::vector<byte> deriveKeyFromPassword(const std::string& password,
                                        const std::vector<byte>& salt,
                                        int iterations,
                                        int keyLength) {
    std::vector<byte> key(keyLength);
    pbkdf2_hmac_sha256(
        reinterpret_cast<const byte*>(password.c_str()), password.length(),
        salt.data(), salt.size(),
        iterations, keyLength, key.data()
    );
    return key;
}

}