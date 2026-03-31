#include "./syncVariantGenerator.h"
/*------------------------------- 32 bits -------------------------------*/
inline uint32_t reverse32(uint32_t PCMBits){
    return reverseBits32(PCMBits);
}

inline uint32_t real32(uint32_t PCMBits){
    return PCMBits;
}

inline uint32_t opposite32(uint32_t PCMBits){
    return ~PCMBits;
}

inline uint32_t reverseOpposite32(uint32_t PCMBits){
    return ~reverse32(PCMBits);
}
/*---------------------------------- 24 bits -----------------------------*/
inline uint32_t reverse24(uint32_t PCMBits){
    return reverseBits24(PCMBits);
}

inline uint32_t real24(uint32_t PCMBits){
    return PCMBits;
}

inline uint32_t opposite24(uint32_t PCMBits){
    return ~PCMBits;
}

inline uint32_t reverseOpposite24(uint32_t PCMBits){
    return ~reverse32(PCMBits);
}

/*--------------------------------- 16 bits --------------------------------*/
inline uint16_t reverse16(uint16_t PCMBits){
    return reverseBits16(PCMBits);
}

inline uint16_t real16(uint16_t PCMBits){
    return PCMBits;
}

inline uint16_t opposite16(uint16_t PCMBits){
    return ~PCMBits;
}

inline uint16_t reverseOpposite16(uint16_t PCMBits){
    return ~reverse32(PCMBits);
}

/*---------------------------------- 8 Bits ------------------------------*/

inline uint8_t reverse8(uint8_t PCMBits){
    return reverseBits8(PCMBits);
}

inline uint8_t real8(uint8_t PCMBits){
    return PCMBits;
}

inline uint8_t opposite8(uint8_t PCMBits){
    return ~PCMBits;
}

inline uint8_t reverseOpposite8(uint8_t PCMBits){
    return ~reverse32(PCMBits);
}