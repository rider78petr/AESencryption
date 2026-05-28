#pragma once

#include "BigInt.hpp"
#include <string>

namespace RSA {

struct PublicKey {
    BigInt n;
    BigInt e;
    
    std::string toHex() const;
    static PublicKey fromHex(const std::string& n_hex, const std::string& e_hex);
};

struct PrivateKey {
    BigInt n;
    BigInt d;
    
    std::string toHex() const;
    static PrivateKey fromHex(const std::string& n_hex, const std::string& d_hex);
};

struct KeyPair {
    PublicKey publicKey;
    PrivateKey privateKey;
};

KeyPair generateKeyPair(int keyBits = 2048);

}
