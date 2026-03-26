#include <cstdint>
#include "ringBuffer.h"
#include <stdint.h>
struct syncVariants{
    uint32_t v[4];
};

inline uint32_t reverseBits32(uint32_t x){
#if __has_builtin(__builtin_bitreverse32)
    return __builtin_bitreverse32(x);
#else
    // Fallback manual bit reversal for old compilers
    x = ((x >> 1) & 0x55555555) | ((x & 0x55555555) << 1);
    x = ((x >> 2) & 0x33333333) | ((x & 0x33333333) << 2);
    x = ((x >> 4) & 0x0F0F0F0F) | ((x & 0x0F0F0F0F) << 4);
    x = ((x >> 8) & 0x00FF00FF) | ((x & 0x00FF00FF) << 8);
    return (x >> 16) | (x << 16);
#endif
}

syncVariants generateVariants(uint32_t pattern){
    syncVariants s;
    s.v[0] = pattern;
    s.v[1] = ~pattern;
    s.v[2] = reverseBits32(pattern);
    s.v[3] = ~s.v[2];
    return s;
}