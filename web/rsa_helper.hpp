#pragma once

#include "../include/RSAkeys.hpp"
#include "../include/RSAcipher.hpp"
#include <string>
#include <map>

namespace WebRSA {

struct RSAResult {
    bool success;
    std::string result;
    std::string error;
};

struct KeyPairResult {
    bool success;
    std::string publicKeyN;
    std::string publicKeyE;
    std::string privateKeyN;
    std::string privateKeyD;
    std::string error;
};

KeyPairResult generateKeys();

RSAResult encryptWithPublicKey(const std::string& plaintext,
                               const std::string& publicKeyN_hex,
                               const std::string& publicKeyE_hex);

RSAResult decryptWithPrivateKey(const std::string& ciphertext,
                                const std::string& privateKeyN_hex,
                                const std::string& privateKeyD_hex);

}
