#pragma once
#include <cstdint>
#include "ringBuffer.h"
#include <stdint.h>
struct syncVariants{
    uint32_t v[4];
};

inline uint32_t reverseBits32(uint32_t x){
#if defined(__has_builtin) && __has_builtin(__builtin_bitreverse32)
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

inline uint32_t reverseBits24(uint32_t x){
#if defined(__has_builtin) && __has_builtin(__builtin_bitreverse24)
    return __builtin_bitreverse24(x);
#else 
    x &= 0x00FFFFFF;

    x = ((x >> 1)  & 0x00555555) | ((x & 0x00555555) << 1);
    x = ((x >> 2)  & 0x00333333) | ((x & 0x00333333) << 2);
    x = ((x >> 4)  & 0x000F0F0F) | ((x & 0x000F0F0F) << 4);
    x = ((x >> 8)  & 0x0000FFFF) | ((x & 0x0000FFFF) << 8);

    return (x >> 8) & 0x00FFFFFF;
#endif
}

inline uint16_t reverseBits16(uint16_t x){
#if defined(__has_builtin) && __has_builtin(__builtin_bitreverse16)
    return __builtin_bitreverse16(x);
#else 
    x = ((x >> 1) & 0x5555) | ((x & 0x5555) << 1);
    x = ((x >> 2) & 0x3333) | ((x & 0x3333) << 2);
    x = ((x >> 4) & 0x0F0F) | ((x & 0x0F0F) << 4);
    x = (x >> 8) | (x << 8);
    return x&0xFFFF;
#endif
}

inline uint8_t reverseBits8(uint8_t x) {
#if defined(__has_builtin) && __has_builtin(__builtin_bitreverse8)
    return __builtin_bitreverse8(x);
#else
    x = ((x >> 1) & 0x55) | ((x & 0x55) << 1); // swap adjacent bits
    x = ((x >> 2) & 0x33) | ((x & 0x33) << 2); // swap pairs
    x = ((x >> 4) & 0x0F) | ((x & 0x0F) << 4); // swap nibbles
    return x;
#endif
}

inline syncVariants generateVariants32(uint32_t pattern){
    syncVariants s;
    s.v[0] = pattern;
    s.v[1] = ~pattern;
    s.v[2] = reverseBits32(pattern);
    s.v[3] = ~s.v[2];
    return s;
}

inline syncVariants generateVariants24(uint32_t pattern){
    syncVariants s;
    s.v[0] = pattern;
    s.v[1] = ~pattern;
    s.v[2] = reverseBits24(pattern);
    s.v[3] = ~s.v[2];
    return s;
}

inline syncVariants generateVariants16(uint32_t pattern){
    syncVariants s;
    s.v[0] = pattern;
    s.v[1] = ~pattern;
    s.v[2] = reverseBits16(pattern);
    s.v[3] = ~s.v[2];
    return s;
}
