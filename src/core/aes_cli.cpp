#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>

#include "../../include/AEScipher.hpp"
#include "../../include/keyExpansion.hpp"
#include "../../include/pbkdf2.hpp"

using namespace AES;

void printBytes(const std::vector<byte>& data, const char* label = nullptr) {
    if (label) std::cout << label << ": ";
    for (byte b : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)b;
    }
    std::cout << std::dec << std::endl;
}

void printBlock(const byte* data, const char* label) {
    std::cout << label << ": ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

bool compareBytes(const byte* a, const byte* b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void encryptWithPassword(const std::string& plaintext, 
                         const std::string& password,
                         std::vector<byte>& ciphertext,
                         std::vector<byte>& salt) {
    salt = generateSalt(16);
    
    std::vector<byte> key = deriveKeyFromPassword(password, salt, 10000, 16);
    
    size_t originalLen = plaintext.length();
    size_t paddedLen = ((originalLen / 16) + 1) * 16;
    std::vector<byte> plainBytes(paddedLen, 0);
    
    for (size_t i = 0; i < originalLen; ++i) {
        plainBytes[i] = static_cast<byte>(plaintext[i]);
    }
    
    byte padValue = static_cast<byte>(paddedLen - originalLen);
    for (size_t i = originalLen; i < paddedLen; ++i) {
        plainBytes[i] = padValue;
    }
    
    byte roundKeys[11][16];
    keyExpansion(key.data(), roundKeys);
    
    ciphertext.resize(paddedLen);
    for (size_t offset = 0; offset < paddedLen; offset += 16) {
        encryptBlock(plainBytes.data() + offset, 
                     ciphertext.data() + offset, 
                     roundKeys);
    }
}

bool decryptWithPassword(const std::vector<byte>& ciphertext,
                         const std::string& password,
                         const std::vector<byte>& salt,
                         std::string& plaintext) {
    if (ciphertext.empty() || ciphertext.size() % 16 != 0) {
        return false;
    }
    
    std::vector<byte> key = deriveKeyFromPassword(password, salt, 10000, 16);
    
    byte roundKeys[11][16];
    keyExpansion(key.data(), roundKeys);
    
    std::vector<byte> decrypted(ciphertext.size());
    for (size_t offset = 0; offset < ciphertext.size(); offset += 16) {
        decryptBlock(ciphertext.data() + offset, 
                     decrypted.data() + offset, 
                     roundKeys);
    }
    
    if (decrypted.empty()) return false;
    
    byte padValue = decrypted.back();
    if (padValue < 1 || padValue > 16) return false;
    
    for (size_t i = decrypted.size() - padValue; i < decrypted.size(); ++i) {
        if (decrypted[i] != padValue) return false;
    }
    
    plaintext.assign(reinterpret_cast<char*>(decrypted.data()), 
                     decrypted.size() - padValue);
    return true;
}

bool runFIPSTest() {
    std::cout << "\n=== FIPS 197 Test ===\n" << std::endl;
    
    byte key[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    
    byte plaintext[16] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    
    byte expectedCiphertext[16] = {
        0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
        0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a
    };
    
    byte ciphertext[16];
    byte decrypted[16];
    byte roundKeys[11][16];
    
    keyExpansion(key, roundKeys);
    
    encryptBlock(plaintext, ciphertext, roundKeys);
    
    bool encryptOk = compareBytes(ciphertext, expectedCiphertext, 16);
    
    decryptBlock(ciphertext, decrypted, roundKeys);
    
    bool decryptOk = compareBytes(decrypted, plaintext, 16);
    
    printBlock(plaintext, "Plaintext");
    printBlock(key, "Key");
    printBlock(ciphertext, "Computed ciphertext");
    printBlock(expectedCiphertext, "Expected ciphertext");
    
    std::cout << "\nEncryption test: " << (encryptOk ? "PASSED" : "FAILED") << std::endl;
    std::cout << "Decryption test: " << (decryptOk ? "PASSED" : "FAILED") << std::endl;
    
    return encryptOk && decryptOk;
}

void demoPasswordEncryption() {
    std::cout << "\n=== Password-Based Encryption Demo ===\n" << std::endl;
    
    std::string password;
    std::string plaintext;
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    std::cout << "Enter text to encrypt: ";
    std::getline(std::cin, plaintext);
    
    std::vector<byte> ciphertext;
    std::vector<byte> salt;
    
    encryptWithPassword(plaintext, password, ciphertext, salt);
    
    std::cout << "\n--- Encryption Result ---" << std::endl;
    printBytes(salt, "Salt (hex)");
    printBytes(ciphertext, "Ciphertext (hex)");
    
    std::string decrypted;
    if (decryptWithPassword(ciphertext, password, salt, decrypted)) {
        std::cout << "\n--- Decryption Verification ---" << std::endl;
        std::cout << "Decrypted text: " << decrypted << std::endl;
        
        if (plaintext == decrypted) {
            std::cout << "✓ Round-trip test: PASSED" << std::endl;
        } else {
            std::cout << "✗ Round-trip test: FAILED" << std::endl;
        }
    } else {
        std::cout << "\n✗ Decryption failed!" << std::endl;
    }
}

void interactiveDecrypt() {
    std::cout << "\n=== Interactive Decryption ===\n" << std::endl;
    
    std::string password;
    std::string saltHex;
    std::string cipherHex;
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    std::cout << "Enter salt (hex): ";
    std::getline(std::cin, saltHex);
    
    std::cout << "Enter ciphertext (hex): ";
    std::getline(std::cin, cipherHex);
    
    std::vector<byte> salt;
    for (size_t i = 0; i + 1 < saltHex.length(); i += 2) {
        salt.push_back(static_cast<byte>(
            std::stoi(saltHex.substr(i, 2), nullptr, 16)));
    }
    
    std::vector<byte> ciphertext;
    for (size_t i = 0; i + 1 < cipherHex.length(); i += 2) {
        ciphertext.push_back(static_cast<byte>(
            std::stoi(cipherHex.substr(i, 2), nullptr, 16)));
    }
    
    std::string plaintext;
    if (decryptWithPassword(ciphertext, password, salt, plaintext)) {
        std::cout << "\n--- Decryption Result ---" << std::endl;
        std::cout << "Decrypted text: " << plaintext << std::endl;
    } else {
        std::cout << "\nDecryption failed! Wrong password or corrupted data." << std::endl;
    }
}

void printHelp() {
    std::cout << "\n============================================================" << std::endl;
    std::cout << "     AES-128 Console Encryption Tool" << std::endl;
    std::cout << "============================================================\n" << std::endl;
    std::cout << "Available commands:\n" << std::endl;
    std::cout << "  help                      - Show this help" << std::endl;
    std::cout << "  test                      - Run FIPS 197 compliance test" << std::endl;
    std::cout << "  demo                      - Demonstrate password-based encryption" << std::endl;
    std::cout << "  encrypt <text> <password> - Quick encrypt (one-liner)" << std::endl;
    std::cout << "  decrypt                   - Interactive decryption" << std::endl;
    std::cout << "\nExamples:\n" << std::endl;
    std::cout << "  ./aes_cli test" << std::endl;
    std::cout << "  ./aes_cli demo" << std::endl;
    std::cout << "  ./aes_cli encrypt \"Hello World\" \"myPassword123\"" << std::endl;
    std::cout << "  ./aes_cli decrypt" << std::endl;
    std::cout << "============================================================\n" << std::endl;
}

void quickEncrypt(const std::string& plaintext, const std::string& password) {
    std::vector<byte> ciphertext;
    std::vector<byte> salt;
    
    encryptWithPassword(plaintext, password, ciphertext, salt);
    
    std::cout << "\n--- Encryption Result ---" << std::endl;
    printBytes(salt, "Salt");
    printBytes(ciphertext, "Ciphertext");
    std::cout << "\nTo decrypt, use:\n  ./aes_cli decrypt" << std::endl;
    std::cout << "And enter:\n  Password: " << password << std::endl;
    std::cout << "  Salt: "; printBytes(salt);
    std::cout << "  Ciphertext: "; printBytes(ciphertext);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }
    
    std::string command = argv[1];
    
    if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
    }
    else if (command == "test") {
        runFIPSTest();
    }
    else if (command == "demo") {
        std::cin.ignore();
        demoPasswordEncryption();
    }
    else if (command == "encrypt" && argc >= 4) {
        std::string plaintext = argv[2];
        std::string password = argv[3];
        quickEncrypt(plaintext, password);
    }
    else if (command == "encrypt" && argc < 4) {
        std::cout << "Usage: ./aes_cli encrypt <text> <password>" << std::endl;
    }
    else if (command == "decrypt") {
        std::cin.ignore();
        interactiveDecrypt();
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
        printHelp();
    }
    
    return 0;
}