#include "BigInt.hpp"
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  КОНСТРУКТОРЫ
// ════════════════════════════════════════════════════════════════

BigInt::BigInt() {
    // По умолчанию — число 0. Храним как [0].
    limbs.push_back(0);
}

BigInt::BigInt(uint64_t value) {
    // 64-битное число разбиваем на два 32-битных разряда.
    limbs.push_back(static_cast<uint32_t>(value & 0xFFFFFFFFu));
    uint32_t high = static_cast<uint32_t>(value >> 32);
    if (high != 0) {
        limbs.push_back(high);
    }
}

BigInt::BigInt(const std::string& decimal) {
    // Превращаем десятичную строку в BigInt.
    // Алгоритм: result = result * 10 + цифра
    *this = BigInt(0);
    BigInt ten(10);
    for (char c : decimal) {
        if (c < '0' || c > '9') {
            throw std::invalid_argument("Not a decimal digit: " + std::string(1, c));
        }
        *this = (*this) * ten + BigInt(static_cast<uint64_t>(c - '0'));
    }
    normalize();
}

BigInt BigInt::fromBytes(const std::vector<uint8_t>& bytes) {
    // Байты идут в big-endian (старший байт первый).
    // Собираем число побайтно: result = result * 256 + byte
    BigInt result(0);
    BigInt b256(256);
    for (uint8_t byte : bytes) {
        result = result * b256 + BigInt(static_cast<uint64_t>(byte));
    }
    return result;
}

BigInt BigInt::fromHex(const std::string& hex) {
    BigInt result(0);
    BigInt b16(16);
    for (char c : hex) {
        int digit;
        if (c >= '0' && c <= '9')      digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        else continue; // пропускаем пробелы и переносы строк
        result = result * b16 + BigInt(static_cast<uint64_t>(digit));
    }
    return result;
}

// ════════════════════════════════════════════════════════════════
//  ПРЕОБРАЗОВАНИЯ ОБРАТНО В СТРОКИ И БАЙТЫ
// ════════════════════════════════════════════════════════════════

std::string BigInt::toDecimal() const {
    if (isZero()) return "0";
    // Делим число на 10 в цикле, собираем остатки — это цифры (в обратном порядке).
    std::string result;
    BigInt n = *this;
    BigInt ten(10);
    while (!n.isZero()) {
        BigInt r = n % ten;
        // r.limbs[0] — цифра 0..9
        result += static_cast<char>('0' + r.limbs[0]);
        n = n / ten;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string BigInt::toHex() const {
    if (isZero()) return "0";
    std::string result;
    // Идём от старшего limb к младшему. Каждый limb = 8 hex-цифр.
    for (int i = (int)limbs.size() - 1; i >= 0; --i) {
        char buf[9];
        if (i == (int)limbs.size() - 1) {
            snprintf(buf, sizeof(buf), "%x", limbs[i]);   // без ведущих нулей
        } else {
            snprintf(buf, sizeof(buf), "%08x", limbs[i]); // с ведущими нулями
        }
        result += buf;
    }
    return result;
}

std::vector<uint8_t> BigInt::toBytes() const {
    // Каждый limb — это 4 байта, big-endian.
    std::vector<uint8_t> bytes;
    for (int i = (int)limbs.size() - 1; i >= 0; --i) {
        bytes.push_back((limbs[i] >> 24) & 0xFF);
        bytes.push_back((limbs[i] >> 16) & 0xFF);
        bytes.push_back((limbs[i] >> 8)  & 0xFF);
        bytes.push_back( limbs[i]        & 0xFF);
    }
    // Убираем ведущие нулевые байты
    while (bytes.size() > 1 && bytes.front() == 0) {
        bytes.erase(bytes.begin());
    }
    return bytes;
}

// ════════════════════════════════════════════════════════════════
//  СРАВНЕНИЯ
// ════════════════════════════════════════════════════════════════

bool BigInt::operator==(const BigInt& o) const {
    return limbs == o.limbs;
}

bool BigInt::operator!=(const BigInt& o) const {
    return !(*this == o);
}

bool BigInt::operator<(const BigInt& o) const {
    // Если разная длина — сравниваем по длине.
    if (limbs.size() != o.limbs.size()) {
        return limbs.size() < o.limbs.size();
    }
    // Длины равны — сравниваем со старшего разряда.
    for (int i = (int)limbs.size() - 1; i >= 0; --i) {
        if (limbs[i] != o.limbs[i]) {
            return limbs[i] < o.limbs[i];
        }
    }
    return false; // равны
}

bool BigInt::operator>(const BigInt& o) const  { return o < *this; }
bool BigInt::operator<=(const BigInt& o) const { return !(o < *this); }
bool BigInt::operator>=(const BigInt& o) const { return !(*this < o); }

bool BigInt::isZero() const {
    return limbs.size() == 1 && limbs[0] == 0;
}

bool BigInt::isOne() const {
    return limbs.size() == 1 && limbs[0] == 1;
}

bool BigInt::isEven() const {
    return (limbs[0] & 1u) == 0;
}

// ════════════════════════════════════════════════════════════════
//  АРИФМЕТИКА
// ════════════════════════════════════════════════════════════════

BigInt BigInt::operator+(const BigInt& o) const {
    // Сложение в столбик с переносом.
    BigInt result;
    result.limbs.clear();
    size_t maxLen = std::max(limbs.size(), o.limbs.size());
    uint64_t carry = 0;
    for (size_t i = 0; i < maxLen || carry; ++i) {
        uint64_t sum = carry;
        if (i < limbs.size())   sum += limbs[i];
        if (i < o.limbs.size()) sum += o.limbs[i];
        result.limbs.push_back(static_cast<uint32_t>(sum & 0xFFFFFFFFu));
        carry = sum >> 32;
    }
    if (result.limbs.empty()) result.limbs.push_back(0);
    return result;
}

BigInt BigInt::operator-(const BigInt& o) const {
    // Вычитание в столбик с заёмом. Предполагается *this >= o.
    if (*this < o) {
        throw std::runtime_error("BigInt subtraction would be negative");
    }
    BigInt result;
    result.limbs.clear();
    int64_t borrow = 0;
    for (size_t i = 0; i < limbs.size(); ++i) {
        int64_t diff = (int64_t)limbs[i] - borrow;
        if (i < o.limbs.size()) diff -= o.limbs[i];
        if (diff < 0) {
            diff += (1LL << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.limbs.push_back(static_cast<uint32_t>(diff));
    }
    result.normalize();
    return result;
}

BigInt BigInt::operator*(const BigInt& o) const {
    // Умножение "столбиком". Каждое limbs[i] * o.limbs[j] кладём в позицию i+j.
    BigInt result;
    result.limbs.assign(limbs.size() + o.limbs.size(), 0);
    for (size_t i = 0; i < limbs.size(); ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < o.limbs.size(); ++j) {
            uint64_t cur = (uint64_t)result.limbs[i + j]
                         + (uint64_t)limbs[i] * o.limbs[j]
                         + carry;
            result.limbs[i + j] = static_cast<uint32_t>(cur & 0xFFFFFFFFu);
            carry = cur >> 32;
        }
        // Не забыть положить итоговый carry
        size_t k = i + o.limbs.size();
        while (carry) {
            uint64_t cur = (uint64_t)result.limbs[k] + carry;
            result.limbs[k] = static_cast<uint32_t>(cur & 0xFFFFFFFFu);
            carry = cur >> 32;
            ++k;
        }
    }
    result.normalize();
    return result;
}

BigInt BigInt::operator/(const BigInt& o) const {
    // Двоичное деление в столбик.
    // Идея: сдвигаем "остаток" влево по биту, добавляем очередной бит делимого,
    // если остаток >= делителя — вычитаем и ставим бит частного = 1.
    if (o.isZero()) throw std::runtime_error("Division by zero");
    if (*this < o) return BigInt(0);

    BigInt quotient;
    quotient.limbs.assign(limbs.size(), 0);
    BigInt remainder(0);

    for (int i = bitLength() - 1; i >= 0; --i) {
        remainder = remainder << 1;
        if (getBit(i)) {
            // прибавляем 1 к младшему биту
            remainder.limbs[0] |= 1u;
        }
        if (remainder >= o) {
            remainder = remainder - o;
            // ставим i-й бит в частном
            quotient.limbs[i / 32] |= (1u << (i % 32));
        }
    }
    quotient.normalize();
    return quotient;
}

BigInt BigInt::operator%(const BigInt& o) const {
    // То же самое что деление, только возвращаем остаток.
    if (o.isZero()) throw std::runtime_error("Modulo by zero");
    if (*this < o) return *this;

    BigInt remainder(0);
    for (int i = bitLength() - 1; i >= 0; --i) {
        remainder = remainder << 1;
        if (getBit(i)) {
            remainder.limbs[0] |= 1u;
        }
        if (remainder >= o) {
            remainder = remainder - o;
        }
    }
    remainder.normalize();
    return remainder;
}

// ════════════════════════════════════════════════════════════════
//  БИТОВЫЕ ОПЕРАЦИИ
// ════════════════════════════════════════════════════════════════

BigInt BigInt::operator<<(int shift) const {
    if (shift == 0) return *this;
    BigInt result;
    result.limbs.clear();
    int wholeLimbs = shift / 32;  // сдвиг на целые разряды
    int bitShift   = shift % 32;  // сдвиг внутри разряда

    // Сначала добавляем wholeLimbs нулей снизу
    for (int i = 0; i < wholeLimbs; ++i) result.limbs.push_back(0);

    // Затем переносим биты с учётом сдвига внутри разряда
    uint32_t carry = 0;
    for (size_t i = 0; i < limbs.size(); ++i) {
        uint64_t val = ((uint64_t)limbs[i] << bitShift) | carry;
        result.limbs.push_back(static_cast<uint32_t>(val & 0xFFFFFFFFu));
        carry = static_cast<uint32_t>(val >> 32);
    }
    if (carry) result.limbs.push_back(carry);
    result.normalize();
    return result;
}

BigInt BigInt::operator>>(int shift) const {
    if (shift == 0) return *this;
    BigInt result;
    result.limbs.clear();
    int wholeLimbs = shift / 32;
    int bitShift   = shift % 32;

    if (wholeLimbs >= (int)limbs.size()) return BigInt(0);

    // Берём limbs[wholeLimbs..end] и сдвигаем их вправо на bitShift
    for (size_t i = wholeLimbs; i < limbs.size(); ++i) {
        uint32_t low  = limbs[i] >> bitShift;
        uint32_t high = (i + 1 < limbs.size() && bitShift > 0)
                       ? (limbs[i + 1] << (32 - bitShift))
                       : 0;
        result.limbs.push_back(low | high);
    }
    if (result.limbs.empty()) result.limbs.push_back(0);
    result.normalize();
    return result;
}

int BigInt::bitLength() const {
    if (isZero()) return 0;
    uint32_t top = limbs.back();
    int bits = 0;
    while (top) { ++bits; top >>= 1; }
    return (int)(limbs.size() - 1) * 32 + bits;
}

bool BigInt::getBit(int index) const {
    int limbIdx = index / 32;
    int bitIdx  = index % 32;
    if (limbIdx >= (int)limbs.size()) return false;
    return (limbs[limbIdx] >> bitIdx) & 1u;
}

// ════════════════════════════════════════════════════════════════
//  УТИЛИТА
// ════════════════════════════════════════════════════════════════

void BigInt::normalize() {
    // Убираем ведущие нули, но оставляем хотя бы один разряд (для числа 0).
    while (limbs.size() > 1 && limbs.back() == 0) {
        limbs.pop_back();
    }
}

} // namespace RSA
