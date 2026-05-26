#include "../../include/BigIntMath.hpp"
#include <stdexcept>

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  modPow — быстрое возведение в степень по модулю
// ════════════════════════════════════════════════════════════════

BigInt modPow(const BigInt& base, const BigInt& exp, const BigInt& modulus) {
    if (modulus.isOne()) {
        return BigInt(0);
    }
    
    BigInt result(1);
    BigInt base_copy = base % modulus;
    BigInt exp_copy = exp;
    
    while (!exp_copy.isZero()) {
        // Если младший бит = 1, умножаем result на base
        if (!exp_copy.isEven()) {
            result = (result * base_copy) % modulus;
        }
        exp_copy = exp_copy >> 1;
        base_copy = (base_copy * base_copy) % modulus;
    }
    
    return result;
}

// ════════════════════════════════════════════════════════════════
//  gcd — наибольший общий делитель
// ════════════════════════════════════════════════════════════════

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

// ════════════════════════════════════════════════════════════════
//  modInverse — модульное обратное (без отрицательных чисел)
// ════════════════════════════════════════════════════════════════

// Используем алгоритм где все промежуточные значения положительные.
// Идея: работаем с парами (старое значение, новое значение) и всегда держим оба положительными.
//
// Стандартный extended Euclidean даёт x, y такие что a*x + b*y = gcd.
// Но x может быть "отрицательным" — то есть на самом деле x представляет (-x).
// 
// Чтобы избежать отрицательных чисел, используем модификацию:
// храним коэффициенты как пары (положительная_часть, отрицательная_часть),
// или работаем по модулю m с самого начала.
//
// Самый простой способ: модульное обратное через возведение в степень.
// Если modulus — простое число, то по малой теореме Ферма:
//   a^(modulus-1) ≡ 1 (mod modulus)
//   a^(modulus-2) ≡ a^(-1) (mod modulus)
//
// Но φ(n) = (p-1)*(q-1) НЕ простое. Поэтому нужен другой подход.
//
// Решение: используем итеративный extended GCD, но коэффициенты считаем по модулю m.

BigInt modInverse(const BigInt& a, const BigInt& modulus) {
    // Проверяем что gcd(a, modulus) = 1
    if (gcd(a, modulus) != BigInt(1)) {
        throw std::runtime_error("modular inverse does not exist (gcd != 1)");
    }
    
    // Итеративный extended Euclidean algorithm.
    // Работаем с коэффициентами по модулю modulus, чтобы они всегда были положительные.
    //
    // На каждом шаге:
    //   old_r * old_s ≡ старое значение (mod modulus)
    //   r * s ≡ новое значение (mod modulus)
    //
    // В конце old_r = gcd = 1, и old_s — это искомое обратное.
    
    BigInt old_r = a;
    BigInt r = modulus;
    BigInt old_s(1);
    BigInt s(0);
    
    while (!r.isZero()) {
        BigInt quotient = old_r / r;
        
        // (old_r, r) = (r, old_r - quotient * r)
        BigInt new_r = old_r - (quotient * r);
        old_r = r;
        r = new_r;
        
        // (old_s, s) = (s, old_s - quotient * s) mod modulus
        // Чтобы избежать отрицательных: old_s - (quotient * s) mod modulus
        // = (old_s + modulus - (quotient * s) mod modulus) mod modulus
        BigInt qs = (quotient * s) % modulus;
        BigInt new_s;
        if (old_s >= qs) {
            new_s = old_s - qs;
        } else {
            // old_s - qs было бы отрицательным; добавляем modulus
            new_s = (old_s + modulus) - qs;
        }
        old_s = s;
        s = new_s;
    }
    
    // old_r должен быть = 1 (gcd)
    // old_s — это искомое обратное
    return old_s % modulus;
}

// ════════════════════════════════════════════════════════════════
//  extendedGcd — оставляем для совместимости, но больше не используем для modInverse
// ════════════════════════════════════════════════════════════════

BigInt extendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y) {
    // Простая версия (только GCD, без коэффициентов)
    x = BigInt(1);
    y = BigInt(0);
    return gcd(a, b);
}

} // namespace RSA
