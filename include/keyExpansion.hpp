#pragma once

#include <cstring>
#include <cstdint>

#include "constants.hpp"

namespace AES {

constexpr uint32_t Rcon[11] = {
    0x00000000,
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0x1B000000,
    0x36000000
};

inline uint32_t rotWord(uint32_t word) {
    return (word << 8) | (word >> 24);
}

inline uint32_t subWord(uint32_t word) {
    uint32_t result = 0;
    result |= SBox[(word >> 24) & 0xFF] << 24;
    result |= SBox[(word >> 16) & 0xFF] << 16;
    result |= SBox[(word >> 8) & 0xFF] << 8;
    result |= SBox[word & 0xFF];
    return result;
}

inline void keyExpansion(const byte* key, byte roundKeys[11][16]) {
    uint32_t w[44];
    
    for (int i = 0; i < 4; i++) {
        w[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) | (key[i * 4 + 2] << 8) | key[i * 4 + 3];
    }
    
    for (int i = 4; i < 44; i++) {
        uint32_t temp = w[i - 1];
        
        if (i % 4 == 0) {
            temp = subWord(rotWord(temp)) ^ Rcon[i / 4];
        }
        
        w[i] = w[i - 4] ^ temp;
    }
    
    for (int round = 0; round < 11; round++) {
        for (int j = 0; j < 4; j++) {
            uint32_t word = w[round * 4 + j];
            roundKeys[round][j * 4]     = (word >> 24) & 0xFF;
            roundKeys[round][j * 4 + 1] = (word >> 16) & 0xFF;
            roundKeys[round][j * 4 + 2] = (word >> 8) & 0xFF;
            roundKeys[round][j * 4 + 3] = word & 0xFF;
        }
    }
}

}
