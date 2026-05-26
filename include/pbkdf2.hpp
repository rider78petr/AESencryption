#pragma once

#include <vector>
#include <string>
#include <cstddef>

namespace AES {

using byte = unsigned char;

std::vector<byte> generateSalt(size_t length = 16);

void pbkdf2_hmac_sha256(const byte* password, size_t passwordLen,
                        const byte* salt, size_t saltLen,
                        int iterations, int keyLength,
                        byte* output);

std::vector<byte> deriveKeyFromPassword(const std::string& password,
                                        const std::vector<byte>& salt,
                                        int iterations,
                                        int keyLength);

}