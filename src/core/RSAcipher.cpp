#include "../../include/RSAcipher.hpp"
#include "../../include/BigIntMath.hpp"
#include "../../include/sha256.hpp"
#include "base64.hpp"
#include <cstring>
#include <stdexcept>
#include <random>
#include <chrono>
#include <sstream>

namespace RSA {

BigInt encryptNumber(const BigInt& message, const PublicKey& pubKey) {
    return modPow(message, pubKey.e, pubKey.n);
}

BigInt decryptNumber(const BigInt& ciphertext, const PrivateKey& privKey) {
    return modPow(ciphertext, privKey.d, privKey.n);
}

static std::mt19937& getRng() {
    static std::mt19937 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    return rng;
}

static uint8_t randomByte() {
    static std::uniform_int_distribution<int> dist(0, 255);
    return static_cast<uint8_t>(dist(getRng()));
}

static int maxBlockSize(int keyBytes) {
    return keyBytes - 33;
}

static std::vector<uint8_t> padBlock(const std::vector<uint8_t>& block, int keyBytes) {
    if ((int)block.size() > maxBlockSize(keyBytes)) {
        throw std::runtime_error("Block too large for RSA key size");
    }
    
    std::vector<uint8_t> padded(keyBytes, 0);
    
    padded[0] = 0x00;
    
    for (int i = 1; i <= 31; ++i) {
        uint8_t b = 0;
        while (b == 0) b = randomByte();
        padded[i] = b;
    }
    
    padded[32] = 0x00;
    
    std::memcpy(padded.data() + 33, block.data(), block.size());
    
    return padded;
}

static std::vector<uint8_t> unpadBlock(const std::vector<uint8_t>& padded, int keyBytes) {
    if ((int)padded.size() != keyBytes) {
        throw std::runtime_error("Invalid padded block size");
    }
    
    size_t sep = 0;
    for (size_t i = 1; i < padded.size(); ++i) {
        if (padded[i] == 0x00) {
            sep = i;
            break;
        }
    }
    
    if (sep == 0 || sep + 1 > padded.size()) {
        throw std::runtime_error("Invalid padding: separator not found");
    }
    
    return std::vector<uint8_t>(padded.begin() + sep + 1, padded.end());
}

std::string encryptText(const std::string& plaintext, const PublicKey& pubKey) {
    int keyBytes = (pubKey.n.bitLength() + 7) / 8;
    int blockSize = maxBlockSize(keyBytes);
    
    if (blockSize <= 0) {
        throw std::runtime_error("RSA key too small");
    }
    
    std::vector<uint8_t> data(plaintext.begin(), plaintext.end());
    
    std::string result;
    size_t offset = 0;
    
    while (offset < data.size()) {
        size_t len = std::min((size_t)blockSize, data.size() - offset);
        std::vector<uint8_t> block(data.begin() + offset, data.begin() + offset + len);
        offset += len;
        
        std::vector<uint8_t> padded = padBlock(block, keyBytes);
        
        BigInt m = BigInt::fromBytes(padded);
        
        if (m >= pubKey.n) {
            throw std::runtime_error("Padded block >= n, use larger key");
        }
        
        BigInt c = encryptNumber(m, pubKey);
        
        std::vector<uint8_t> cBytes = c.toBytes();
        while ((int)cBytes.size() < keyBytes) {
            cBytes.insert(cBytes.begin(), 0x00);
        }
        
        if (!result.empty()) result += ":";
        result += WebUtils::base64_encode(cBytes);
    }
    
    return result;
}

std::string decryptText(const std::string& ciphertext, const PrivateKey& privKey) {
    int keyBytes = (privKey.n.bitLength() + 7) / 8;
    
    std::vector<std::string> blocks;
    std::stringstream ss(ciphertext);
    std::string token;
    while (std::getline(ss, token, ':')) {
        if (!token.empty()) blocks.push_back(token);
    }
    
    if (blocks.empty()) {
        throw std::runtime_error("Empty ciphertext");
    }
    
    std::string plaintext;
    
    for (const auto& b64block : blocks) {
        std::vector<uint8_t> cBytes = WebUtils::base64_decode(b64block);
        
        BigInt c = BigInt::fromBytes(cBytes);
        BigInt m = decryptNumber(c, privKey);
        
        std::vector<uint8_t> padded = m.toBytes();
        while ((int)padded.size() < keyBytes) {
            padded.insert(padded.begin(), 0x00);
        }
        
        std::vector<uint8_t> block = unpadBlock(padded, keyBytes);
        
        plaintext += std::string(block.begin(), block.end());
    }
    
    return plaintext;
}

}
