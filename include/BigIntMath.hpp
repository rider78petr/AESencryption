#pragma once

#include "BigInt.hpp"

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  Модульная арифметика
// ════════════════════════════════════════════════════════════════

/**
 * Быстрое возведение в степень по модулю: (base^exp) mod modulus
 * Использует метод "binary exponentiation" (возведение в квадрат).
 * Временная сложность: O(log(exp) * log(modulus)^2)
 */
BigInt modPow(const BigInt& base, const BigInt& exp, const BigInt& modulus);

/**
 * Наибольший общий делитель gcd(a, b).
 * Использует алгоритм Евклида.
 */
BigInt gcd(const BigInt& a, const BigInt& b);

/**
 * Расширенный алгоритм Евклида.
 * Находит gcd(a, b) и коэффициенты x, y такие что: a*x + b*y = gcd(a, b)
 * Возвращает gcd, x и y передаются по ссылке.
 */
BigInt extendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y);

/**
 * Модульное обратное: найти x такой что (a * x) mod modulus = 1
 * Существует только если gcd(a, modulus) = 1.
 * Выбросит исключение если обратного не существует.
 */
BigInt modInverse(const BigInt& a, const BigInt& modulus);

} // namespace RSA
