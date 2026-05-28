#include "../../include/PrimeGen.hpp"
#include "../../include/BigIntMath.hpp"
#include <random>
#include <chrono>
#include <gmp.h>

namespace RSA {

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

static bool divisibleBySmallPrime(const BigInt& n) {
    for (int i = 0; i < SMALL_PRIMES_COUNT; ++i) {
        BigInt p(SMALL_PRIMES[i]);
        if (n == p) return false;
        if ((n % p).isZero()) return true;
    }
    return false;
}

BigInt generateRandomBits(int bitLength) {
    static gmp_randstate_t rng_state;
    static bool initialized = false;
    
    if (!initialized) {
        gmp_randinit_mt(rng_state);
        unsigned long seed = std::chrono::high_resolution_clock::now()
                                .time_since_epoch().count();
        gmp_randseed_ui(rng_state, seed);
        initialized = true;
    }
    
    BigInt result;
    mpz_urandomb(result.value, rng_state, bitLength);
    mpz_setbit(result.value, bitLength - 1);
    
    return result;
}

bool millerRabin(const BigInt& n, int rounds) {
    if (n < BigInt(2)) return false;
    if (n == BigInt(2) || n == BigInt(3)) return true;
    if (n.isEven()) return false;
    
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
        
        if (x == BigInt(1) || x == n_minus_1) continue;
        
        bool composite = true;
        for (int i = 0; i < r - 1; ++i) {
            x = (x * x) % n;
            if (x == n_minus_1) {
                composite = false;
                break;
            }
        }
        
        if (composite) return false;
    }
    
    return true;
}

BigInt generateRandomPrime(int bitLength) {
    while (true) {
        BigInt candidate = generateRandomBits(bitLength);
        
        if (candidate.isEven()) {
            candidate = candidate + BigInt(1);
        }
        
        for (int attempts = 0; attempts < 10000; ++attempts) {
            if (!divisibleBySmallPrime(candidate)) {
                if (millerRabin(candidate, 20)) {
                    return candidate;
                }
            }
            candidate = candidate + BigInt(2);
        }
    }
}

}
