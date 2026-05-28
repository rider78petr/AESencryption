#include "crypto_helper.hpp"
#include "base64.hpp"
#include "config.hpp"
#include "AEScipher.hpp"
#include "keyExpansion.hpp"
#include "pbkdf2.hpp"
#include <sstream>
#include <cstring>

namespace WebCrypto {

    EncryptResult encryptText(const std::string& plaintext, const std::string& password) {
        EncryptResult result;
        try {
            std::vector<byte> salt = AES::generateSalt(WebConfig::SALT_SIZE_BYTES);
            std::vector<byte> key  = AES::deriveKeyFromPassword(
                password, salt, WebConfig::PBKDF2_ITERATIONS, WebConfig::KEY_SIZE_BYTES);

            size_t originalLen = plaintext.length();
            size_t paddedLen   = ((originalLen / 16) + 1) * 16;
            std::vector<byte> plainBytes(paddedLen, 0);
            for (size_t i = 0; i < originalLen; ++i)
                plainBytes[i] = static_cast<byte>(plaintext[i]);
            byte padValue = static_cast<byte>(paddedLen - originalLen);
            for (size_t i = originalLen; i < paddedLen; ++i)
                plainBytes[i] = padValue;

            byte roundKeys[11][16];
            AES::keyExpansion(key.data(), roundKeys);

            std::vector<byte> ciphertext(paddedLen);
            for (size_t offset = 0; offset < paddedLen; offset += 16)
                AES::encryptBlock(plainBytes.data() + offset,
                                  ciphertext.data() + offset, roundKeys);

            result.ciphertextBase64 = WebUtils::base64_encode(ciphertext);
            result.saltBase64       = WebUtils::base64_encode(salt);
            result.success          = true;
        } catch (const std::exception& e) {
            result.success = false;
            result.error   = e.what();
        }
        return result;
    }

    DecryptResult decryptText(const std::string& ciphertextB64,
                              const std::string& saltB64,
                              const std::string& password) {
        DecryptResult result;
        try {
            std::vector<byte> ciphertext = WebUtils::base64_decode(ciphertextB64);
            std::vector<byte> salt       = WebUtils::base64_decode(saltB64);

            if (ciphertext.empty() || ciphertext.size() % 16 != 0) {
                result.success = false;
                result.error   = "Invalid ciphertext size. Must be multiple of 16 bytes.";
                return result;
            }

            std::vector<byte> key = AES::deriveKeyFromPassword(
                password, salt, WebConfig::PBKDF2_ITERATIONS, WebConfig::KEY_SIZE_BYTES);

            byte roundKeys[11][16];
            AES::keyExpansion(key.data(), roundKeys);

            std::vector<byte> decrypted(ciphertext.size());
            for (size_t offset = 0; offset < ciphertext.size(); offset += 16)
                AES::decryptBlock(ciphertext.data() + offset,
                                  decrypted.data() + offset, roundKeys);

            byte padValue = decrypted.back();
            if (padValue < 1 || padValue > 16) {
                result.success = false;
                result.error   = "Invalid padding - wrong password or corrupted data";
                return result;
            }
            for (size_t i = decrypted.size() - padValue; i < decrypted.size(); ++i) {
                if (decrypted[i] != padValue) {
                    result.success = false;
                    result.error   = "Padding verification failed - wrong password";
                    return result;
                }
            }

            result.plaintext = std::string(reinterpret_cast<char*>(decrypted.data()),
                                           decrypted.size() - padValue);
            result.success = true;
        } catch (const std::exception& e) {
            result.success = false;
            result.error   = e.what();
        }
        return result;
    }

    FIPSTestResult runFIPSTest() {
        FIPSTestResult result;

        byte key[16] = {
            0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
            0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
        };
        byte plaintext[16] = {
            0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
            0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
        };
        byte expected[16] = {
            0x69,0xc4,0xe0,0xd8,0x6a,0x7b,0x04,0x30,
            0xd8,0xcd,0xb7,0x80,0x70,0xb4,0xc5,0x5a
        };

        byte ciphertext[16], decrypted[16], roundKeys[11][16];
        AES::keyExpansion(key, roundKeys);
        AES::encryptBlock(plaintext, ciphertext, roundKeys);
        AES::decryptBlock(ciphertext, decrypted, roundKeys);

        bool encryptOk = std::memcmp(ciphertext, expected, 16) == 0;
        bool decryptOk = std::memcmp(decrypted, plaintext, 16) == 0;

        result.passed      = encryptOk && decryptOk;
        result.keyHex      = WebUtils::bytesToHex(key, 16);
        result.plaintextHex= WebUtils::bytesToHex(plaintext, 16);
        result.expectedHex = WebUtils::bytesToHex(expected, 16);
        result.computedHex = WebUtils::bytesToHex(ciphertext, 16);

        std::stringstream ss;
        ss << "=== FIPS 197 COMPLIANCE TEST ===\n\n";
        ss << "Key:       " << result.keyHex       << "\n";
        ss << "Plaintext: " << result.plaintextHex  << "\n";
        ss << "Expected:  " << result.expectedHex   << "\n";
        ss << "Computed:  " << result.computedHex   << "\n\n";
        ss << "Encryption: " << (encryptOk ? "PASSED" : "FAILED") << "\n";
        ss << "Decryption: " << (decryptOk ? "PASSED" : "FAILED") << "\n";
        ss << "\nOverall: "  << (result.passed ? "PASSED" : "FAILED");
        result.details = ss.str();
        return result;
    }

    std::string getVersionInfo() {
        std::stringstream ss;
        ss << "AES-128 Implementation\n";
        ss << "  Block Size:       128 bits\n";
        ss << "  Key Size:         128 bits\n";
        ss << "  Rounds:           10\n";
        ss << "  PBKDF2 Iterations:" << WebConfig::PBKDF2_ITERATIONS << "\n";
        ss << "  KDF:              PBKDF2-HMAC-SHA256\n";
        ss << "  Padding:          PKCS#7\n";
        return ss.str();
    }
}
