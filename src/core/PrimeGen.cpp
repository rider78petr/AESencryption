#include "../../include/PrimeGen.hpp"
#include "../../include/BigIntMath.hpp"
#include <random>
#include <chrono>

namespace RSA {

// Маленькие простые числа для быстрой фильтрации.
// Если кандидат делится на любое из них — он составной (кроме случая когда он сам = это простое).
static const uint32_t SMALL_PRIMES[] = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
    71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
    149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
    307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
    389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569,
    571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647,
    653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
    751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839,
    853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
    947, 953, 967, 971, 977, 983, 991, 997
};
static const int SMALL_PRIMES_COUNT = sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

// Быстрая проверка: делится ли число на маленькое простое?
// Возвращает true если число точно составное.
static bool divisibleBySmallPrime(const BigInt& n) {
    for (int i = 0; i < SMALL_PRIMES_COUNT; ++i) {
        BigInt p(SMALL_PRIMES[i]);
        if (n == p) return false;  // само простое
        if ((n % p).isZero()) return true;  // делится — составное
    }
    return false;
}

// ════════════════════════════════════════════════════════════════
//  Генерация случайных битов
// ════════════════════════════════════════════════════════════════

BigInt generateRandomBits(int bitLength) {
    static std::mt19937_64 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFFu);
    
    BigInt result;
    result.limbs.clear();
    
    int fullLimbs = bitLength / 32;
    int remainingBits = bitLength % 32;
    
    for (int i = 0; i < fullLimbs; ++i) {
        result.limbs.push_back(dist(rng));
    }
    
    if (remainingBits > 0) {
        uint32_t partial = dist(rng);
        uint32_t mask = (1u << remainingBits) - 1;
        partial &= mask;
        partial |= (1u << (remainingBits - 1));
        result.limbs.push_back(partial);
    } else {
        // если bitLength делится на 32, гарантируем что старший бит = 1
        result.limbs.back() |= (1u << 31);
    }
    
    result.normalize();
    return result;
}

// ════════════════════════════════════════════════════════════════
//  Тест Миллера-Рабина
// ════════════════════════════════════════════════════════════════

bool millerRabin(const BigInt& n, int rounds) {
    if (n < BigInt(2)) return false;
    if (n == BigInt(2) || n == BigInt(3)) return true;
    if (n.isEven()) return false;
    
    // n - 1 = 2^r * d
    BigInt n_minus_1 = n - BigInt(1);
    BigInt d = n_minus_1;
    int r = 0;
    
    while (d.isEven()) {
        d = d >> 1;
        r++;
    }
    
    for (int round = 0; round < rounds; ++round) {
        BigInt a = generateRandomBits(n.bitLength() - 1);
        if (a < BigInt(2)) a = BigInt(2);
        if (a >= n_minus_1) a = a % (n_minus_1 - BigInt(2)) + BigInt(2);
        
        BigInt x = modPow(a, d, n);
        
        if (x == BigInt(1) || x == n_minus_1) {
            continue;
        }
        
        bool composite = true;
        for (int i = 0; i < r - 1; ++i) {
            x = (x * x) % n;  // вместо modPow(x, 2, n) — быстрее
            if (x == n_minus_1) {
                composite = false;
                break;
            }
        }
        
        if (composite) {
            return false;
        }
    }
    
    return true;
}

// ════════════════════════════════════════════════════════════════
//  Генерация простого числа (с оптимизациями)
// ════════════════════════════════════════════════════════════════

BigInt generateRandomPrime(int bitLength) {
    while (true) {
        BigInt candidate = generateRandomBits(bitLength);
        
        // Делаем нечётным
        if (candidate.isEven()) {
            candidate = candidate + BigInt(1);
        }
        
        // Ищем простое, увеличивая candidate на 2 каждый раз
        // Сначала отсеиваем дешёвой проверкой делимости на маленькие простые,
        // потом — дорогим Миллером-Рабином.
        for (int attempts = 0; attempts < 10000; ++attempts) {
            if (!divisibleBySmallPrime(candidate)) {
                if (millerRabin(candidate, 20)) {  // 20 раундов достаточно для 2^-40 надёжности
                    return candidate;
                }
            }
            candidate = candidate + BigInt(2);
        }
        // Если за 10000 попыток не нашли — генерируем новое случайное число
    }
}

} // namespace RSA