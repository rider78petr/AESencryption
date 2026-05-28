#pragma once

#include "BigInt.hpp"

namespace RSA {

/**
 * Тест Миллера-Рабина на простоту.
 * Вероятностный тест: если вернёт false — число точно составное.
 * Если вернёт true — число *вероятно* простое с ошибкой < 4^(-rounds).
 *
 * rounds — количество раундов. Для криптографии обычно 40-50 достаточно.
 */
bool millerRabin(const BigInt& n, int rounds = 40);

/**
 * Генерирует случайное простое число с заданной длиной в битах.
 * bitLength — желаемая длина в битах (например 1024 для RSA-2048).
 */
BigInt generateRandomPrime(int bitLength);

/**
 * Генерирует случайное число заданной длины в битах.
 * Используется как основание для поиска простого числа.
 */
BigInt generateRandomBits(int bitLength);

} // namespace RSA
