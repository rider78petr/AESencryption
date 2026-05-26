#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace RSA {

class BigInt {
public:
    // Число хранится как массив 32-битных "разрядов" в основании 2^32.
    // limbs[0] — младший разряд, limbs[последний] — старший.
    // Пример: число (limbs[1] * 2^32 + limbs[0]).
    std::vector<uint32_t> limbs;

    // ── Конструкторы ──────────────────────────────────────────────
    BigInt();                              // 0
    BigInt(uint64_t value);                // из обычного числа
    BigInt(const std::string& decimal);    // из десятичной строки
    static BigInt fromBytes(const std::vector<uint8_t>& bytes); // из байтов (big-endian)
    static BigInt fromHex(const std::string& hex);              // из hex-строки

    // ── Преобразования обратно ────────────────────────────────────
    std::string toDecimal() const;
    std::string toHex() const;
    std::vector<uint8_t> toBytes() const;   // big-endian

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
    BigInt operator-(const BigInt& o) const;   // предполагается *this >= o
    BigInt operator*(const BigInt& o) const;
    BigInt operator/(const BigInt& o) const;
    BigInt operator%(const BigInt& o) const;

    // ── Битовые операции ──────────────────────────────────────────
    BigInt operator<<(int shift) const;
    BigInt operator>>(int shift) const;
    int bitLength() const;                // сколько бит занимает число
    bool getBit(int index) const;         // бит по индексу (0 = младший)

    // ── Утилиты ───────────────────────────────────────────────────
    void normalize();                     // убрать ведущие нули в limbs
};

} // namespace RSA
