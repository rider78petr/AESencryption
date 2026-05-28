#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <gmp.h>

namespace RSA {

/**
 * Обёртка над GMP mpz_t для удобной работы.
 * Внутри использует библиотеку GMP для быстрой математики больших чисел.
 * Все криптографические алгоритмы (Miller-Rabin, RSA, OAEP) реализованы
 * нашим кодом, используя этот класс.
 */
class BigInt {
public:
    mpz_t value;  // GMP big integer

    // ── Конструкторы и деструктор ─────────────────────────────────
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

    // ── Преобразования ────────────────────────────────────────────
    std::string toDecimal() const;
    std::string toHex() const;
    std::vector<uint8_t> toBytes() const;

    // ── Сравнения ─────────────────────────────────────────────────
    bool operator==(const BigInt& o) const;
    bool operator!=(const BigInt& o) const;
    bool operator<(const BigInt& o) const;
    bool operator>(const BigInt& o) const;
    bool operator<=(const BigInt& o) const;
    bool operator>=(const BigInt& o) const;
    bool isZero() const;
    bool isOne() const;
    bool isEven() const;

    // ── Арифметика ────────────────────────────────────────────────
    BigInt operator+(const BigInt& o) const;
    BigInt operator-(const BigInt& o) const;
    BigInt operator*(const BigInt& o) const;
    BigInt operator/(const BigInt& o) const;
    BigInt operator%(const BigInt& o) const;

    // ── Битовые операции ──────────────────────────────────────────
    BigInt operator<<(int shift) const;
    BigInt operator>>(int shift) const;
    int bitLength() const;
    bool getBit(int index) const;

    void normalize() {} // не нужно для GMP, оставлено для совместимости
};

} // namespace RSA