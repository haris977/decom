#include "../include/syncFindWithMask.h"
#include "../include/syncVariantGenerator.h"
#include <iomanip>
#include <fstream>
using namespace std;
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

// ofstream outputfile("minorframe.csv",ios::trunc);
// int findMinorSync32_auto(RingBuffer& rb, uint32_t sync){
//     if (rb.available()<5) return -1;
//     syncVariants variants = generateVariants32(sync);
//     // printf("This is synfindwithmask : 1: %08X, 2: %08X, 3: %08X, 4: %08X\n",variants.v[0],variants.v[1],variants.v[2],variants.v[3]);
//     int locked = -1;
//     uint64_t temp = ((uint64_t)rb.get(0) << 56) |
//                     ((uint64_t)rb.get(1) << 48) |
//                     ((uint64_t)rb.get(2) << 32) |
//                     ((uint64_t)rb.get(3) << 24) |
//                     ((uint64_t)rb.get(4) << 16) |
//                     ((uint64_t)rb.get(5) << 8)  |
//                     rb.get(6);
//                     // std::cout << std::hex << std::setfill('0');
//                     // std::cout << "  : " << std::setw(16) << temp << std::endl;
//                     // printf("%02X %02X %02X %02X %02X %02X %02X %02X\n",rb.get(0),rb.get(1),rb.get(2),rb.get(3),rb.get(4),rb.get(5),rb.get(6),rb.get(7));
//         while (rb.available()>=5){
//         if (locked ==-1){
//             for (int shift = 0;shift<8;shift++){
//                 uint32_t candidate = (temp>>(8-shift)) & 0xFFFFFFFF;
//                 for (int k = 0;k<4;k++){
//                     if (candidate == variants.v[k]){
//                         locked = k;
//                         return shift;
//                     }
//                 }
//                 if (locked !=-1) break;
//             }
//         }
//         else{
//             uint32_t target = variants.v[locked];
//             printf("syncFindWithMask else : why we came here :");
//             while (rb.available() >=5){
//                 if (((temp >> 8) & 0xFFFFFFFF) == target) return 0;
//                 if (((temp >> 7) & 0xFFFFFFFF) == target) return 1;
//                 if (((temp >> 6) & 0xFFFFFFFF) == target) return 2;
//                 if (((temp >> 5) & 0xFFFFFFFF) == target) return 3;
//                 if (((temp >> 4) & 0xFFFFFFFF) == target) return 4;
//                 if (((temp >> 3) & 0xFFFFFFFF) == target) return 5;
//                 if (((temp >> 2) & 0xFFFFFFFF) == target) return 6;
//                 if (((temp >> 1) & 0xFFFFFFFF) == target) return 7;
//                 if (rb.available() >=5 ){
//                     temp = ((temp<<8)& 0xFFFFFFFFFFULL) | rb.get(4);
//                 }
//             }
//             return -1;
//         }
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(0)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(1)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(2)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(3)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(4)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(5)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(6)<<" ";
//         // outputfile<<hex<<setw(2)<<setfill('0')<<(int)rb.get(7)<<" ";
//         // outputfile<<"\n";
//         // printf("%02X %02X %02X %02X %02X %02X %02X %02X\n",rb.get(0),rb.get(1),rb.get(2),rb.get(3),rb.get(4),rb.get(5),rb.get(6),rb.get(7));

//         rb.advance(1);
//     }
//     // outputfile.close();
//     return -1;
// }

int findMinorSync32_auto(RingBuffer& rb, uint32_t sync) {
    if (rb.available() < 5) return -1;  // 40 bits minimum
    // printf("syncfindwithmask: enter minor frame read index %d\n",rb.readindex);
    syncVariants variants = generateVariants32(sync);

    // Load initial 40 bits (5 bytes)
    uint64_t temp =
        ((uint64_t)rb.get(0) << 32) |
        ((uint64_t)rb.get(1) << 24) |
        ((uint64_t)rb.get(2) << 16) |
        ((uint64_t)rb.get(3) << 8)  |
        ((uint64_t)rb.get(4));
        // printf("temp: %016X : \n",temp);
        
        int bitPos = 0;
        while (rb.available() >= 5) {
            // check all 8 bit shifts inside current 5-byte window
            for (int shift = 7; shift >=0; shift--) {
                uint32_t candidate = (temp >> (7 - shift)) & 0xFFFFFFFF;
                // printf("after minor frame cadidate : %8X ",candidate);
            for (int k = 0; k < 4; k++) {
                if (candidate == variants.v[k]) {
                    // printf("syncfindwithmask: candidate: %08X, exit index: %d, bit pos: %d \n",candidate,rb.readindex,bitPos);
                    //how to get k and bitpos at same time with bit shift yeh : 
                    return (k<<16)|bitPos;
                    // rb.advance(bitPos/8);
                    // return shift;   // exact bit offset
                }
            }
        }
        // slide by 1 byte (8 bits)
        rb.advance(1);
        // cout<<"why you are here :"<<endl;
        bitPos += 8;
        if (rb.available() < 5) break;
        temp =
            ((uint64_t)rb.get(0) << 32) |
            ((uint64_t)rb.get(1) << 24) |
            ((uint64_t)rb.get(2) << 16) |
            ((uint64_t)rb.get(3) << 8)  |
            ((uint64_t)rb.get(4));
    }

    return -1;
}

int findMinorSync24_auto(RingBuffer& rb, uint32_t sync){
    if (rb.available()<4) return -1;
    syncVariants variants = generateVariants24(sync);
    int locked = -1;
    uint64_t temp = ((uint32_t)rb.get(0) << 24) |
        ((uint32_t)rb.get(1) << 16) |
        ((uint32_t)rb.get(2) << 8)  |
        rb.get(3);
    while (rb.available()>=4){
        if (locked ==-1){
            for (int shift = 0;shift<8;shift++){
                uint32_t candidate = (temp>>(8-shift)) & 0xFFFFFF;
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
                if (((temp >> 8) & 0xFFFFFF) == target) return 0;
                if (((temp >> 7) & 0xFFFFFF) == target) return 1;
                if (((temp >> 6) & 0xFFFFFF) == target) return 2;
                if (((temp >> 5) & 0xFFFFFF) == target) return 3;
                if (((temp >> 4) & 0xFFFFFF) == target) return 4;
                if (((temp >> 3) & 0xFFFFFF) == target) return 5;
                if (((temp >> 2) & 0xFFFFFF) == target) return 6;
                if (((temp >> 1) & 0xFFFFFF) == target) return 7;
                rb.advance(1);
                if (rb.available() >=4 ){
                    temp = ((temp<<8)& 0xFFFFFFFF) | rb.get(4);
                }
            }
            return -1;
        }
    }
    return -1;
}

int findMinorSync16_auto(RingBuffer& rb, uint32_t sync){
    if (rb.available()<3) return -1;
    syncVariants variants = generateVariants16(sync);
    int locked = -1;
    uint64_t temp = ((uint16_t)rb.get(0) << 8) |
        ((uint16_t)rb.get(1));
    while (rb.available()>=3){
        if (locked ==-1){
            for (int shift = 0;shift<8;shift++){
                uint16_t candidate = (temp>>(8-shift)) & 0xFFFF;
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
            uint16_t target = variants.v[locked];
            while (rb.available() >=5){
                if (((temp >> 8) & 0xFFFF) == target) return 0;
                if (((temp >> 7) & 0xFFFF) == target) return 1;
                if (((temp >> 6) & 0xFFFF) == target) return 2;
                if (((temp >> 5) & 0xFFFF) == target) return 3;
                if (((temp >> 4) & 0xFFFF) == target) return 4;
                if (((temp >> 3) & 0xFFFF) == target) return 5;
                if (((temp >> 2) & 0xFFFF) == target) return 6;
                if (((temp >> 1) & 0xFFFF) == target) return 7;
                rb.advance(1);
                if (rb.available() >=3 ){
                    temp = ((temp<<8)& 0xFFFFFF) | rb.get(4);
                }
            }
            return -1;
        }
    }
    return -1;
}