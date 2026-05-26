// test_aes.cpp
#include <iostream>
#include <iomanip>
#include <cstring>

#include "AEScipher.hpp"
#include "keyExpansion.hpp"

using namespace AES;

void printHex(const byte* data, const char* label) {
    std::cout << label << ": ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

bool compareBytes(const byte* a, const byte* b, int len) {
    for (int i = 0; i < len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

int main() {
    std::cout << "=== AES-128 Testing according to FIPS 197 ===\n" << std::endl;

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
    std::cout << "Key expansion completed successfully." << std::endl;

    encryptBlock(plaintext, ciphertext, roundKeys);
    printHex(ciphertext, "Computed ciphertext");

    bool encryptOk = compareBytes(ciphertext, expectedCiphertext, 16);
    if (encryptOk) {
        std::cout << "Encryption: correct!" << std::endl;
    } else {
        std::cout << "Encryption: error!" << std::endl;
        printHex(expectedCiphertext, "Expected ciphertext");
    }

    decryptBlock(ciphertext, decrypted, roundKeys);
    printHex(decrypted, "Decrypted text");

    bool decryptOk = compareBytes(decrypted, plaintext, 16);
    if (decryptOk) {
        std::cout << "Decryption: correct!" << std::endl;
    } else {
        std::cout << "Decryption: error!" << std::endl;
        printHex(plaintext, "Original plaintext");
    }

    std::cout << "\n=== Testing completed ===" << std::endl;
    return 0;
}