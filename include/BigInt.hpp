#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <gmp.h>

namespace RSA {

class BigInt {
public:
    mpz_t value;

    BigInt();
    BigInt(uint64_t v);
    BigInt(const std::string& decimal);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;
    ~BigInt();

    static BigInt fromBytes(const std::vector<uint8_t>& bytes);
    static BigInt fromHex(const std::string& hex);

    std::string toDecimal() const;
    std::string toHex() const;
    std::vector<uint8_t> toBytes() const;

    bool operator==(const BigInt& o) const;
    bool operator!=(const BigInt& o) const;
    bool operator<(const BigInt& o) const;
    bool operator>(const BigInt& o) const;
    bool operator<=(const BigInt& o) const;
    bool operator>=(const BigInt& o) const;
    bool isZero() const;
    bool isOne() const;
    bool isEven() const;

    BigInt operator+(const BigInt& o) const;
    BigInt operator-(const BigInt& o) const;
    BigInt operator*(const BigInt& o) const;
    BigInt operator/(const BigInt& o) const;
    BigInt operator%(const BigInt& o) const;

    BigInt operator<<(int shift) const;
    BigInt operator>>(int shift) const;
    int bitLength() const;
    bool getBit(int index) const;

    void normalize() {}
};

}
