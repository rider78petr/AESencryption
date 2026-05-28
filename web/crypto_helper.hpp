#pragma once
#include <string>
#include <vector>
#include <memory>

namespace WebCrypto {

    using byte = unsigned char;
    
    struct EncryptResult {
        std::string ciphertextBase64;
        std::string saltBase64;
        bool success;
        std::string error;
    };
    
    struct DecryptResult {
        std::string plaintext;
        bool success;
        std::string error;
    };
    
    struct FIPSTestResult {
        bool passed;
        std::string keyHex;
        std::string plaintextHex;
        std::string expectedHex;
        std::string computedHex;
        std::string details;
    };
    
    /**
     * Encrypts text using password-based key derivation
     */
    EncryptResult encryptText(const std::string& plaintext, const std::string& password);
    
    /**
     * Decrypts text using password and salt
     */
    DecryptResult decryptText(const std::string& ciphertextB64, 
                              const std::string& saltB64, 
                              const std::string& password);
    
    /**
     * Runs FIPS 197 compliance test
     */
    FIPSTestResult runFIPSTest();
    
    /**
     * Returns version information
     */
    std::string getVersionInfo();
}