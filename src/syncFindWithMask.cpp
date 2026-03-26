#include "../include/syncFindWithMask.h"
#include "../include/syncVariantGenerator.h"
uint32_t reverse32(uint32_t x) {
    x = ((x >> 1)  & 0x55555555) | ((x & 0x55555555) << 1);
    x = ((x >> 2)  & 0x33333333) | ((x & 0x33333333) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F) | ((x & 0x0F0F0F0F) << 4);
    x = ((x >> 8)  & 0x00FF00FF) | ((x & 0x00FF00FF) << 8);
    x = (x >> 16) | (x << 16);
    return x;
}

uint16_t reverse16(uint16_t x) {
    x = ((x >> 1) & 0x5555) | ((x & 0x5555) << 1);
    x = ((x >> 2) & 0x3333) | ((x & 0x3333) << 2);
    x = ((x >> 4) & 0x0F0F) | ((x & 0x0F0F) << 4);
    x = (x >> 8) | (x << 8);
    return x;
}

uint32_t reverse24(uint32_t x) {
    x &= 0x00FFFFFF;

    x = ((x >> 1)  & 0x00555555) | ((x & 0x00555555) << 1);
    x = ((x >> 2)  & 0x00333333) | ((x & 0x00333333) << 2);
    x = ((x >> 4)  & 0x000F0F0F) | ((x & 0x000F0F0F) << 4);
    x = ((x >> 8)  & 0x0000FFFF) | ((x & 0x0000FFFF) << 8);

    return (x >> 8) & 0x00FFFFFF;
}

// void parseStream(RingBuffer& rb){
//     const uint32_t sync = 0xFE6B2840;
//     while (rb.available()>=11){
//         uint64_t temp = ((uint64_t)rb.get(0) << 32) |
//             ((uint64_t)rb.get(1) << 24) |
//             ((uint64_t)rb.get(2) << 16) |
//             ((uint64_t)rb.get(3) << 8)  |
//             rb.get(4);
//         bool found = false;
//         for (int shift = 0;shift<8;shift++){
//             uint32_t candidate = (temp>>(8-shift))&0xFFFFFFFF;
//             if (candidate == sync){
//                 uint64_t next = ((uint64_t)rb.get(4) << 40) |
//                     ((uint64_t)rb.get(5) << 32) |
//                     ((uint64_t)rb.get(6) << 24) |
//                     ((uint64_t)rb.get(7) << 16) |
//                     ((uint64_t)rb.get(8) << 8)  |
//                     rb.get(9);
//             }
//         }
//     }
// }

int findMinorSync32_rb(RingBuffer& rb, uint32_t sync){
    if (rb.available() <5) return -1;
    uint64_t temp = ((uint64_t)rb.get(0) << 32) |
        ((uint64_t)rb.get(1) << 24) |
        ((uint64_t)rb.get(2) << 16) |
        ((uint64_t)rb.get(3) << 8)  |
        rb.get(4);
    while (rb.available() >=5){
        if (((temp >> 8) & 0xFFFFFFFF) == sync) return 0;
        if (((temp >> 7) & 0xFFFFFFFF) == sync) return 1;
        if (((temp >> 6) & 0xFFFFFFFF) == sync) return 2;
        if (((temp >> 5) & 0xFFFFFFFF) == sync) return 3;
        if (((temp >> 4) & 0xFFFFFFFF) == sync) return 4;
        if (((temp >> 3) & 0xFFFFFFFF) == sync) return 5;
        if (((temp >> 2) & 0xFFFFFFFF) == sync) return 6;
        if (((temp >> 1) & 0xFFFFFFFF) == sync) return 7;
        rb.advance(1);
        if (rb.available() >=5 ){
            temp = ((temp<<8)& 0xFFFFFFFFFFULL) | rb.get(4);
        }
    }
    return -1;
}

int findMinorSync32_auto(RingBuffer& rb, uint32_t sync){
    if (rb.available()<5) return -1;
    syncVariants variants = generateVariants(sync);
    int locked = -1;
    uint64_t temp = ((uint64_t)rb.get(0) << 32) |
        ((uint64_t)rb.get(1) << 24) |
        ((uint64_t)rb.get(2) << 16) |
        ((uint64_t)rb.get(3) << 8)  |
        rb.get(4);
    while (rb.available()>=5){
        if (locked ==-1){
            for (int shift = 0;shift<8;shift++){
                uint32_t candidate = (temp>>(8-shift)) & 0xFFFFFFFF;
                for (int k = 0;k<4;k++){
                    if (candidate == variants.v[k]){
                        locked = k;
                        break;
                    }
                }
                if (locked !=-1) break;
            }
        }
        else{
            uint32_t target = variants.v[locked];
            while (rb.available() >=5){
                if (((temp >> 8) & 0xFFFFFFFF) == target) return 0;
                if (((temp >> 7) & 0xFFFFFFFF) == target) return 1;
                if (((temp >> 6) & 0xFFFFFFFF) == target) return 2;
                if (((temp >> 5) & 0xFFFFFFFF) == target) return 3;
                if (((temp >> 4) & 0xFFFFFFFF) == target) return 4;
                if (((temp >> 3) & 0xFFFFFFFF) == target) return 5;
                if (((temp >> 2) & 0xFFFFFFFF) == target) return 6;
                if (((temp >> 1) & 0xFFFFFFFF) == target) return 7;
                rb.advance(1);
                if (rb.available() >=5 ){
                    temp = ((temp<<8)& 0xFFFFFFFFFFULL) | rb.get(4);
                }
            }
            return -1;
        }
    }
}