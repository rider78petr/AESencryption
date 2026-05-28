#include "../../include/BigInt.hpp"
#include <stdexcept>
#include <cstring>

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  Конструкторы / деструктор
// ════════════════════════════════════════════════════════════════

BigInt::BigInt() {
    mpz_init(value);
}

BigInt::BigInt(uint64_t v) {
    mpz_init(value);
    mpz_import(value, 1, -1, sizeof(uint64_t), 0, 0, &v);
}

BigInt::BigInt(const std::string& decimal) {
    mpz_init(value);
    if (mpz_set_str(value, decimal.c_str(), 10) != 0) {
        mpz_clear(value);
        throw std::invalid_argument("Invalid decimal string: " + decimal);
    }
}

BigInt::BigInt(const BigInt& other) {
    mpz_init(value);
    mpz_set(value, other.value);
}

BigInt::BigInt(BigInt&& other) noexcept {
    // Перемещаем: забираем себе value, а у other инициализируем пустое
    mpz_init(value);
    mpz_swap(value, other.value);
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        mpz_set(value, other.value);
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        mpz_swap(value, other.value);
    }
    return *this;
}

BigInt::~BigInt() {
    mpz_clear(value);
}

BigInt BigInt::fromBytes(const std::vector<uint8_t>& bytes) {
    BigInt result;
    if (bytes.empty()) return result;
    // big-endian, 1 byte per word, most significant word first
    mpz_import(result.value, bytes.size(), 1, 1, 1, 0, bytes.data());
    return result;
}

BigInt BigInt::fromHex(const std::string& hex) {
    BigInt result;
    // Удаляем пробелы и переносы строк
    std::string clean;
    for (char c : hex) {
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            clean += c;
        }
    }
    if (mpz_set_str(result.value, clean.c_str(), 16) != 0) {
        throw std::invalid_argument("Invalid hex string");
    }
    return result;
}

// ════════════════════════════════════════════════════════════════
//  Преобразования
// ════════════════════════════════════════════════════════════════

std::string BigInt::toDecimal() const {
    char* str = mpz_get_str(nullptr, 10, value);
    std::string result(str);
    free(str);
    return result;
}

std::string BigInt::toHex() const {
    char* str = mpz_get_str(nullptr, 16, value);
    std::string result(str);
    free(str);
    return result;
}

std::vector<uint8_t> BigInt::toBytes() const {
    if (isZero()) return {0};
    size_t count = (mpz_sizeinbase(value, 2) + 7) / 8;
    std::vector<uint8_t> bytes(count);
    size_t written;
    mpz_export(bytes.data(), &written, 1, 1, 1, 0, value);
    bytes.resize(written);
    return bytes;
}

// ════════════════════════════════════════════════════════════════
//  Сравнения
// ════════════════════════════════════════════════════════════════

bool BigInt::operator==(const BigInt& o) const { return mpz_cmp(value, o.value) == 0; }
bool BigInt::operator!=(const BigInt& o) const { return mpz_cmp(value, o.value) != 0; }
bool BigInt::operator<(const BigInt& o)  const { return mpz_cmp(value, o.value) < 0; }
bool BigInt::operator>(const BigInt& o)  const { return mpz_cmp(value, o.value) > 0; }
bool BigInt::operator<=(const BigInt& o) const { return mpz_cmp(value, o.value) <= 0; }
bool BigInt::operator>=(const BigInt& o) const { return mpz_cmp(value, o.value) >= 0; }

bool BigInt::isZero() const { return mpz_sgn(value) == 0; }
bool BigInt::isOne()  const { return mpz_cmp_ui(value, 1) == 0; }
bool BigInt::isEven() const { return mpz_even_p(value) != 0; }

// ════════════════════════════════════════════════════════════════
//  Арифметика
// ════════════════════════════════════════════════════════════════

BigInt BigInt::operator+(const BigInt& o) const {
    BigInt r;
    mpz_add(r.value, value, o.value);
    return r;
}

BigInt BigInt::operator-(const BigInt& o) const {
    BigInt r;
    mpz_sub(r.value, value, o.value);
    if (mpz_sgn(r.value) < 0) {
        throw std::runtime_error("BigInt subtraction would be negative");
    }
    return r;
}

BigInt BigInt::operator*(const BigInt& o) const {
    BigInt r;
    mpz_mul(r.value, value, o.value);
    return r;
}

BigInt BigInt::operator/(const BigInt& o) const {
    if (o.isZero()) throw std::runtime_error("Division by zero");
    BigInt r;
    mpz_fdiv_q(r.value, value, o.value);
    return r;
}

BigInt BigInt::operator%(const BigInt& o) const {
    if (o.isZero()) throw std::runtime_error("Modulo by zero");
    BigInt r;
    mpz_fdiv_r(r.value, value, o.value);
    return r;
}

// ════════════════════════════════════════════════════════════════
//  Битовые операции
// ════════════════════════════════════════════════════════════════

BigInt BigInt::operator<<(int shift) const {
    BigInt r;
    mpz_mul_2exp(r.value, value, shift);
    return r;
}

BigInt BigInt::operator>>(int shift) const {
    BigInt r;
    mpz_fdiv_q_2exp(r.value, value, shift);
    return r;
}

int BigInt::bitLength() const {
    if (isZero()) return 0;
    return (int)mpz_sizeinbase(value, 2);
}

bool BigInt::getBit(int index) const {
    return mpz_tstbit(value, index) != 0;
}

} // namespace RSA