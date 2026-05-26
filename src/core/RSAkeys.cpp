#include "../../include/RSAkeys.hpp"
#include "../../include/PrimeGen.hpp"
#include "../../include/BigIntMath.hpp"
#include <sstream>

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  PublicKey
// ════════════════════════════════════════════════════════════════

std::string PublicKey::toHex() const {
    return "n:" + n.toHex() + "|e:" + e.toHex();
}

PublicKey PublicKey::fromHex(const std::string& n_hex, const std::string& e_hex) {
    PublicKey key;
    key.n = BigInt::fromHex(n_hex);
    key.e = BigInt::fromHex(e_hex);
    return key;
}

// ════════════════════════════════════════════════════════════════
//  PrivateKey
// ════════════════════════════════════════════════════════════════

std::string PrivateKey::toHex() const {
    return "n:" + n.toHex() + "|d:" + d.toHex();
}

PrivateKey PrivateKey::fromHex(const std::string& n_hex, const std::string& d_hex) {
    PrivateKey key;
    key.n = BigInt::fromHex(n_hex);
    key.d = BigInt::fromHex(d_hex);
    return key;
}

// ════════════════════════════════════════════════════════════════
//  Генерация ключевой пары
// ════════════════════════════════════════════════════════════════

KeyPair generateKeyPair(int keyBits) {
    // Шаг 1: генерируем два простых числа p и q
    // Каждое длиной примерно keyBits/2 бит
    int primeBits = (keyBits + 1) / 2;  // для 2048 это 1024
    
    BigInt p = generateRandomPrime(primeBits);
    BigInt q = generateRandomPrime(primeBits);
    
    // Убеждаемся что p != q
    while (p == q) {
        q = generateRandomPrime(primeBits);
    }
    
    // Шаг 2: вычисляем n = p * q
    BigInt n = p * q;
    
    // Шаг 3: вычисляем функцию Эйлера φ(n) = (p-1) * (q-1)
    BigInt p_minus_1 = p - BigInt(1);
    BigInt q_minus_1 = q - BigInt(1);
    BigInt phi = p_minus_1 * q_minus_1;
    
    // Шаг 4: выбираем публичную экспоненту e
    // Стандартное значение: e = 65537 = 2^16 + 1
    // Это простое число и гарантирует gcd(e, φ(n)) = 1 для большинства n
    BigInt e(65537);
    
    // Проверяем что gcd(e, φ(n)) = 1 (если нет, выбираем другое e)
    // На практике это почти всегда верно для e = 65537
    while (gcd(e, phi) != BigInt(1)) {
        e = e + BigInt(2);  // берём следующее нечётное число
    }
    
    // Шаг 5: вычисляем приватную экспоненту d
    // d = e^(-1) mod φ(n)
    // То есть d такой что (e * d) mod φ(n) = 1
    BigInt d = modInverse(e, phi);
    
    // Шаг 6: формируем ключевую пару
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

} // namespace RSA
