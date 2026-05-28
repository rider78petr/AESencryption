#pragma once

#include "constants.hpp"
#include "gf256.hpp"

namespace AES {

    inline void subBytes(byte* state) {
        for (int i = 0; i < 16; i++) {
            state[i] = SBox[state[i]];
        }
    }

    inline void invSubBytes(byte* state) {
        for (int i = 0; i < 16; i++) {
            state[i] = invSBox[state[i]];
        }
    }

    inline void shiftRows(byte* state) {
        byte tmp[16];
        
        tmp[0]  = state[0];
        tmp[1]  = state[5];
        tmp[2]  = state[10];
        tmp[3]  = state[15];
        
        tmp[4]  = state[4];
        tmp[5]  = state[9];
        tmp[6]  = state[14];
        tmp[7]  = state[3];
        
        tmp[8]  = state[8];
        tmp[9]  = state[13];
        tmp[10] = state[2];
        tmp[11] = state[7];
        
        tmp[12] = state[12];
        tmp[13] = state[1];
        tmp[14] = state[6];
        tmp[15] = state[11];
        
        for (int i = 0; i < 16; i++) {
            state[i] = tmp[i];
        }
    }

    inline void invShiftRows(byte* state) {
        byte tmp[16];
        
        tmp[0]  = state[0];
        tmp[1]  = state[13];
        tmp[2]  = state[10];
        tmp[3]  = state[7];
        
        tmp[4]  = state[4];
        tmp[5]  = state[1];
        tmp[6]  = state[14];
        tmp[7]  = state[11];
        
        tmp[8]  = state[8];
        tmp[9]  = state[5];
        tmp[10] = state[2];
        tmp[11] = state[15];
        
        tmp[12] = state[12];
        tmp[13] = state[9];
        tmp[14] = state[6];
        tmp[15] = state[3];
        
        for (int i = 0; i < 16; i++) {
            state[i] = tmp[i];
        }
    }

    inline void mixColumns(byte* state) {
        byte tmp[16];
        
        for (int col = 0; col < 4; col++) {
            int base = col * 4;
            
            byte b0 = state[base + 0];
            byte b1 = state[base + 1];
            byte b2 = state[base + 2];
            byte b3 = state[base + 3];
            
            tmp[base + 0] = gf_multiply(0x02, b0) ^ gf_multiply(0x03, b1) ^ b2 ^ b3;
            tmp[base + 1] = b0 ^ gf_multiply(0x02, b1) ^ gf_multiply(0x03, b2) ^ b3;
            tmp[base + 2] = b0 ^ b1 ^ gf_multiply(0x02, b2) ^ gf_multiply(0x03, b3);
            tmp[base + 3] = gf_multiply(0x03, b0) ^ b1 ^ b2 ^ gf_multiply(0x02, b3);
        }
        
        for (int i = 0; i < 16; i++) {
            state[i] = tmp[i];
        }
    }

    inline void invMixColumns(byte* state) {
        byte tmp[16];
        
        for (int col = 0; col < 4; col++) {
            int base = col * 4;
            
            byte b0 = state[base + 0];
            byte b1 = state[base + 1];
            byte b2 = state[base + 2];
            byte b3 = state[base + 3];
            
            tmp[base + 0] = gf_multiply(0x0E, b0) ^ gf_multiply(0x0B, b1) ^ gf_multiply(0x0D, b2) ^ gf_multiply(0x09, b3);
            tmp[base + 1] = gf_multiply(0x09, b0) ^ gf_multiply(0x0E, b1) ^ gf_multiply(0x0B, b2) ^ gf_multiply(0x0D, b3);
            tmp[base + 2] = gf_multiply(0x0D, b0) ^ gf_multiply(0x09, b1) ^ gf_multiply(0x0E, b2) ^ gf_multiply(0x0B, b3);
            tmp[base + 3] = gf_multiply(0x0B, b0) ^ gf_multiply(0x0D, b1) ^ gf_multiply(0x09, b2) ^ gf_multiply(0x0E, b3);
        }
        
        for (int i = 0; i < 16; i++) {
            state[i] = tmp[i];
        }
    }

    inline void addRoundKey(byte* state, const byte* roundKey) {
        for (int i = 0; i < 16; i++) {
            state[i] ^= roundKey[i];
        }
    }
    
}
