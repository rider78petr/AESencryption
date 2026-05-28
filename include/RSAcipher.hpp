#pragma once

#include "BigInt.hpp"
#include "RSAkeys.hpp"
#include <vector>
#include <string>

namespace RSA {

BigInt encryptNumber(const BigInt& message, const PublicKey& pubKey);

BigInt decryptNumber(const BigInt& ciphertext, const PrivateKey& privKey);

std::string encryptText(const std::string& plaintext, const PublicKey& pubKey);

std::string decryptText(const std::string& ciphertext, const PrivateKey& privKey);

}
