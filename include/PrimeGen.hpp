#pragma once

#include "BigInt.hpp"

namespace RSA {

bool millerRabin(const BigInt& n, int rounds = 40);

BigInt generateRandomPrime(int bitLength);

BigInt generateRandomBits(int bitLength);

}
