#include "../../include/gf256.hpp"

namespace AES {

unsigned char xtime(unsigned char b) {
    unsigned char result = b << 1;
    if (b & 0x80)
        result ^= 0x1B;
    return result;
}

unsigned char gf_multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char temp = a;
    
    while (b != 0) {
        if (b & 0x01) {
            result ^= temp;
        }
        temp = xtime(temp);
        b >>= 1;
    }
    
    return result;
}

}
