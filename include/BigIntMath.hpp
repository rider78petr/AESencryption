#pragma once

#include "BigInt.hpp"

namespace RSA {

BigInt modPow(const BigInt& base, const BigInt& exp, const BigInt& modulus);

BigInt gcd(const BigInt& a, const BigInt& b);

BigInt extendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y);

BigInt modInverse(const BigInt& a, const BigInt& modulus);

}
