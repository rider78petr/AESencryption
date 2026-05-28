#pragma once

#include "BigInt.hpp"
#include <string>

namespace RSA {

/**
 * Открытый ключ RSA: пара (n, e)
 * n — модуль (произведение двух простых чисел p и q)
 * e — публичная экспонента (обычно 65537)
 */
struct PublicKey {
    BigInt n;   // модуль
    BigInt e;   // публичная экспонента
    
    std::string toHex() const;
    static PublicKey fromHex(const std::string& n_hex, const std::string& e_hex);
};

/**
 * Закрытый ключ RSA: пара (n, d)
 * n — модуль (то же что в публичном ключе)
 * d — приватная экспонента (вычисляется из p, q, e)
 */
struct PrivateKey {
    BigInt n;   // модуль
    BigInt d;   // приватная экспонента
    
    std::string toHex() const;
    static PrivateKey fromHex(const std::string& n_hex, const std::string& d_hex);
};

/**
 * Пара публичный + приватный ключ
 */
struct KeyPair {
    PublicKey publicKey;
    PrivateKey privateKey;
};

/**
 * Генерирует пару RSA-ключей длиной keyBits бит.
 * keyBits обычно 2048 для криптографии.
 * 
 * Алгоритм:
 * 1. Генерируем два случайных простых числа p и q длиной keyBits/2 бит каждое
 * 2. n = p * q
 * 3. φ(n) = (p-1) * (q-1)
 * 4. e = 65537 (стандартная публичная экспонента)
 * 5. d = e^(-1) mod φ(n)  (модульное обратное)
 * 6. Возвращаем PublicKey(n, e) и PrivateKey(n, d)
 */
KeyPair generateKeyPair(int keyBits = 2048);

} // namespace RSA
