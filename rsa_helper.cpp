#include "rsa_helper.hpp"
#include <chrono>

namespace WebRSA {

KeyPairResult generateKeys() {
    KeyPairResult result;
    result.success = false;
    
    try {
        // Генерируем RSA-2048 ключевую пару
        // Это может занять несколько секунд (генерация больших простых чисел)
        auto start = std::chrono::high_resolution_clock::now();
        
        RSA::KeyPair keyPair = RSA::generateKeyPair(2048);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Конвертируем ключи в hex для хранения и передачи
        result.publicKeyN = keyPair.publicKey.n.toHex();
        result.publicKeyE = keyPair.publicKey.e.toHex();
        result.privateKeyN = keyPair.privateKey.n.toHex();
        result.privateKeyD = keyPair.privateKey.d.toHex();
        
        result.success = true;
    } catch (const std::exception& e) {
        result.error = std::string("Key generation failed: ") + e.what();
    }
    
    return result;
}

RSAResult encryptWithPublicKey(const std::string& plaintext,
                               const std::string& publicKeyN_hex,
                               const std::string& publicKeyE_hex) {
    RSAResult result;
    result.success = false;
    
    try {
        if (plaintext.empty()) {
            result.error = "Plaintext cannot be empty";
            return result;
        }
        
        // Восстанавливаем публичный ключ из hex
        RSA::PublicKey pubKey = RSA::PublicKey::fromHex(publicKeyN_hex, publicKeyE_hex);
        
        // Шифруем текст
        std::string ciphertext = RSA::encryptText(plaintext, pubKey);
        
        result.result = ciphertext;
        result.success = true;
    } catch (const std::exception& e) {
        result.error = std::string("Encryption failed: ") + e.what();
    }
    
    return result;
}

RSAResult decryptWithPrivateKey(const std::string& ciphertext,
                                const std::string& privateKeyN_hex,
                                const std::string& privateKeyD_hex) {
    RSAResult result;
    result.success = false;
    
    try {
        if (ciphertext.empty()) {
            result.error = "Ciphertext cannot be empty";
            return result;
        }
        
        // Восстанавливаем приватный ключ из hex
        RSA::PrivateKey privKey = RSA::PrivateKey::fromHex(privateKeyN_hex, privateKeyD_hex);
        
        // Расшифровываем текст
        std::string plaintext = RSA::decryptText(ciphertext, privKey);
        
        result.result = plaintext;
        result.success = true;
    } catch (const std::exception& e) {
        result.error = std::string("Decryption failed: ") + e.what();
    }
    
    return result;
}

} // namespace WebRSA