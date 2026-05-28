#pragma once

#include "AESoperations.hpp"
#include "keyExpansion.hpp"

namespace AES {

inline void encryptBlock(const byte* input, byte* output, const byte roundKeys[11][16]) {
    byte state[16];
    for (int i = 0; i < 16; ++i) state[i] = input[i];

    addRoundKey(state, roundKeys[0]);

    for (int round = 1; round <= 9; ++round) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKeys[round]);
    }

    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKeys[10]);

    for (int i = 0; i < 16; ++i) output[i] = state[i];
}

inline void decryptBlock(const byte* input, byte* output, const byte roundKeys[11][16]) {
    byte state[16];
    for (int i = 0; i < 16; ++i) state[i] = input[i];

    addRoundKey(state, roundKeys[10]);

    for (int round = 9; round >= 1; --round) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, roundKeys[round]);
        invMixColumns(state);
    }

    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKeys[0]);

    for (int i = 0; i < 16; ++i) output[i] = state[i];
}

}
