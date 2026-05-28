#include "../../include/RSAkeys.hpp"
#include "../../include/PrimeGen.hpp"
#include "../../include/BigIntMath.hpp"
#include <sstream>

namespace RSA {

std::string PublicKey::toHex() const {
    return "n:" + n.toHex() + "|e:" + e.toHex();
}

PublicKey PublicKey::fromHex(const std::string& n_hex, const std::string& e_hex) {
    PublicKey key;
    key.n = BigInt::fromHex(n_hex);
    key.e = BigInt::fromHex(e_hex);
    return key;
}

std::string PrivateKey::toHex() const {
    return "n:" + n.toHex() + "|d:" + d.toHex();
}

PrivateKey PrivateKey::fromHex(const std::string& n_hex, const std::string& d_hex) {
    PrivateKey key;
    key.n = BigInt::fromHex(n_hex);
    key.d = BigInt::fromHex(d_hex);
    return key;
}

KeyPair generateKeyPair(int keyBits) {
    int primeBits = (keyBits + 1) / 2;
    
    BigInt p = generateRandomPrime(primeBits);
    BigInt q = generateRandomPrime(primeBits);
    
    while (p == q) {
        q = generateRandomPrime(primeBits);
    }
    
    BigInt n = p * q;
    
    BigInt p_minus_1 = p - BigInt(1);
    BigInt q_minus_1 = q - BigInt(1);
    BigInt phi = p_minus_1 * q_minus_1;
    
    BigInt e(65537);
    
    while (gcd(e, phi) != BigInt(1)) {
        e = e + BigInt(2);
    }
    
    BigInt d = modInverse(e, phi);
    
    PublicKey pubKey;
    pubKey.n = n;
    pubKey.e = e;
    
    PrivateKey privKey;
    privKey.n = n;
    privKey.d = d;
    
    KeyPair keyPair;
    keyPair.publicKey = pubKey;
    keyPair.privateKey = privKey;
    
    return keyPair;
}

}
