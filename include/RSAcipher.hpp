#pragma once

#include "BigInt.hpp"
#include "RSAkeys.hpp"
#include <vector>
#include <string>

namespace RSA {

/**
 * Шифрует одно число (меньше n) публичным ключом.
 * Базовая операция RSA: c = m^e mod n
 */
BigInt encryptNumber(const BigInt& message, const PublicKey& pubKey);

/**
 * Расшифровывает число приватным ключом.
 * Базовая операция RSA: m = c^d mod n
 */
BigInt decryptNumber(const BigInt& ciphertext, const PrivateKey& privKey);

/**
 * Шифрует текст (строку) публичным ключом.
 * 
 * Алгоритм:
 * 1. Преобразуем текст в байты
 * 2. Применяем OAEP padding (безопасное дополнение)
 * 3. Преобразуем дополненные байты в BigInt
 * 4. Шифруем число публичным ключом
 * 5. Преобразуем результат обратно в байты и кодируем в base64
 * 
 * Возвращаем зашифрованный текст в виде base64 строки.
 */
std::string encryptText(const std::string& plaintext, const PublicKey& pubKey);

/**
 * Расшифровывает текст приватным ключом.
 * 
 * Алгоритм:
 * 1. Декодируем base64 строку в байты
 * 2. Преобразуем байты в BigInt
 * 3. Расшифровываем число приватным ключом
 * 4. Преобразуем результат в байты
 * 5. Удаляем OAEP padding
 * 6. Преобразуем байты обратно в текст (строку)
 * 
 * Выбросит исключение если расшифровка не удалась (неверный padding).
 */
std::string decryptText(const std::string& ciphertext, const PrivateKey& privKey);

} // namespace RSA
