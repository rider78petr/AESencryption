#include "../../include/BigIntMath.hpp"
#include <stdexcept>

namespace RSA {

BigInt modPow(const BigInt& base, const BigInt& exp, const BigInt& modulus) {
    if (modulus.isOne()) {
        return BigInt(0);
    }
    
    BigInt result(1);
    BigInt base_copy = base % modulus;
    BigInt exp_copy = exp;
    
    while (!exp_copy.isZero()) {
        if (!exp_copy.isEven()) {
            result = (result * base_copy) % modulus;
        }
        exp_copy = exp_copy >> 1;
        base_copy = (base_copy * base_copy) % modulus;
    }
    
    return result;
}

BigInt gcd(const BigInt& a, const BigInt& b) {
    BigInt x = a;
    BigInt y = b;
    
    while (!y.isZero()) {
        BigInt temp = y;
        y = x % y;
        x = temp;
    }
    
    return x;
}

BigInt modInverse(const BigInt& a, const BigInt& modulus) {
    if (gcd(a, modulus) != BigInt(1)) {
        throw std::runtime_error("modular inverse does not exist (gcd != 1)");
    }
    
    BigInt old_r = a;
    BigInt r = modulus;
    BigInt old_s(1);
    BigInt s(0);
    
    while (!r.isZero()) {
        BigInt quotient = old_r / r;
        
        BigInt new_r = old_r - (quotient * r);
        old_r = r;
        r = new_r;
        
        BigInt qs = (quotient * s) % modulus;
        BigInt new_s;
        if (old_s >= qs) {
            new_s = old_s - qs;
        } else {
            new_s = (old_s + modulus) - qs;
        }
        old_s = s;
        s = new_s;
    }
    
    return old_s % modulus;
}

BigInt extendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y) {
    x = BigInt(1);
    y = BigInt(0);
    return gcd(a, b);
}

}
