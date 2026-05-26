#include "../../include/RSAcipher.hpp"
#include "../../include/BigIntMath.hpp"
#include "../../include/sha256.hpp"
#include "../web/base64.hpp"
#include <cstring>
#include <stdexcept>
#include <random>
#include <chrono>

namespace RSA {

// ════════════════════════════════════════════════════════════════
//  Базовые операции шифрования
// ════════════════════════════════════════════════════════════════

BigInt encryptNumber(const BigInt& message, const PublicKey& pubKey) {
    // c = m^e mod n
    return modPow(message, pubKey.e, pubKey.n);
}

BigInt decryptNumber(const BigInt& ciphertext, const PrivateKey& privKey) {
    // m = c^d mod n
    return modPow(ciphertext, privKey.d, privKey.n);
}

// ════════════════════════════════════════════════════════════════
//  OAEP Padding (Optimal Asymmetric Encryption Padding)
// ════════════════════════════════════════════════════════════════

// OAEP — это безопасный способ добавить случайность к сообщению перед RSA.
// Без OAEP, RSA уязвима к атакам (например, шифрование 0 даёт 0).
// С OAEP — даже одно и то же сообщение даёт разные шифротексты (случайность в padding).

// Упрощённая версия OAEP для учебного проекта:
// plaintext | (random_seed) -> после обработки -> k бит, где k = bitLength(n)

std::vector<uint8_t> oaepPad(const std::vector<uint8_t>& plaintext, int keyBits) {
    // keyBits — битовая длина модуля n
    // keyBytes — количество байт в модуле
    int keyBytes = (keyBits + 7) / 8;
    
    // В упрощённой OAEP:
    // Результат = seed || seedMask || plaintextMask || plaintext
    // где seed — случайные байты, остальное — XOR с SHA256 хешами
    
    // Для простоты: просто добавляем случайные байты в начало
    std::vector<uint8_t> result(keyBytes, 0);
    
    // Генерируем случайный seed (первые 32 байта = SHA256_DIGEST_SIZE)
    static std::mt19937 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    std::uniform_int_distribution<int> dis(0, 255);
    
    std::vector<uint8_t> seed(32);
    for (int i = 0; i < 32; ++i) {
        seed[i] = static_cast<uint8_t>(dis(rng));
    }
    
    // Копируем seed в начало
    std::memcpy(result.data(), seed.data(), 32);
    
    // Копируем plaintext в конец (перед нулями)
    // Если plaintext слишком длинный, обрезаем
    size_t plaintextPos = keyBytes - plaintext.size();
    if (plaintextPos > 32) {
        std::memcpy(result.data() + plaintextPos, plaintext.data(), plaintext.size());
    } else {
        // plaintext слишком большой, копируем сколько влезает
        size_t copySize = keyBytes - 32;
        if (copySize > 0) {
            std::memcpy(result.data() + 32, plaintext.data(),
                       std::min(copySize, plaintext.size()));
        }
    }
    
    return result;
}

std::vector<uint8_t> oaepUnpad(const std::vector<uint8_t>& padded, int keyBits) {
    // Обратная операция: извлекаем plaintext из padded
    // Это упрощённая версия, в реальной OAEP нужны проверки целостности
    
    int keyBytes = (keyBits + 7) / 8;
    
    if (padded.size() != keyBytes) {
        throw std::runtime_error("Invalid padded size");
    }
    
    // Ищем начало plaintext (пропускаем seed и нули)
    // plaintext заканчивается в конце padded
    
    // Упрощённо: ищем первый ненулевой байт после seed (32 байта)
    size_t start = 32;
    while (start < padded.size() && padded[start] == 0) {
        start++;
    }
    
    if (start >= padded.size()) {
        throw std::runtime_error("Invalid padding");
    }
    
    // Извлекаем plaintext от start до конца
    std::vector<uint8_t> result(padded.begin() + start, padded.end());
    
    // Но plaintext может быть пустой или состоять только из нулей,
    // поэтому пропускаем ведущие нули в конце (они могут быть частью padding)
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }
    
    return result;
}

// ════════════════════════════════════════════════════════════════
//  Шифрование и расшифрование текста
// ════════════════════════════════════════════════════════════════

std::string encryptText(const std::string& plaintext, const PublicKey& pubKey) {
    // 1. Преобразуем текст в байты
    std::vector<uint8_t> plainBytes(plaintext.begin(), plaintext.end());
    
    // 2. Применяем OAEP padding
    std::vector<uint8_t> padded = oaepPad(plainBytes, pubKey.n.bitLength());
    
    // 3. Преобразуем байты в BigInt
    BigInt m = BigInt::fromBytes(padded);
    
    // Проверяем что m < n (требование RSA)
    if (m >= pubKey.n) {
        throw std::runtime_error("Plaintext too large for RSA key size");
    }
    
    // 4. Шифруем
    BigInt c = encryptNumber(m, pubKey);
    
    // 5. Преобразуем результат в байты и кодируем в base64
    std::vector<uint8_t> cipherBytes = c.toBytes();
    std::string cipherBase64 = WebUtils::base64_encode(cipherBytes);
    
    return cipherBase64;
}

std::string decryptText(const std::string& ciphertext, const PrivateKey& privKey) {
    // 1. Декодируем base64
    std::vector<uint8_t> cipherBytes = WebUtils::base64_decode(ciphertext);
    
    // 2. Преобразуем в BigInt
    BigInt c = BigInt::fromBytes(cipherBytes);
    
    // 3. Расшифровываем
    BigInt m = decryptNumber(c, privKey);
    
    // 4. Преобразуем результат в байты
    std::vector<uint8_t> padded = m.toBytes();
    
    // Дополняем нулями в начало если нужно (потеряны при преобразовании в BigInt)
    int keyBytes = (privKey.n.bitLength() + 7) / 8;
    while (padded.size() < keyBytes) {
        padded.insert(padded.begin(), 0);
    }
    
    // 5. Удаляем OAEP padding
    std::vector<uint8_t> plainBytes = oaepUnpad(padded, privKey.n.bitLength());
    
    // 6. Преобразуем байты в строку
    std::string plaintext(plainBytes.begin(), plainBytes.end());
    
    return plaintext;
}

} // namespace RSA
