#pragma once
#include <cstdint>
#include "ringBuffer.h"
#include "syncVariantGenerator.h"
#include "ch10Header.h"
uint32_t reverse32(uint32_t x);
uint32_t reverse24(uint32_t x);
uint16_t reverse16(uint16_t x);
void parseStream(RingBuffer& rb);
int findMinorSync32_rb(RingBuffer& rb, uint32_t sync);
// int findMinorSync32_auto(RingBuffer& rb, uint32_t sync);
int findMinorSync24_auto(RingBuffer& rb, uint32_t sync);

int findMinorSync32_auto(RingBuffer& rb, uint32_t sync,int & counter,ch10PrimaryHeader& primary, ch10SecondaryHeader& secondary, channelSpecificData& channel);

inline uint64_t packetMissMatchSync32(RingBuffer& rb, int value){
    uint64_t temp ;
    if (value==3){
        temp = ((uint64_t)rb.get(0)<<32)|
                ((uint64_t)rb.get(1)<<24)|
                ((uint64_t)rb.get(2)<<16);
        temp|=0x6000000000;
    }
    else if (value==2){
        temp = ((uint64_t)rb.get(0)<<32)|
                ((uint64_t)rb.get(1)<<24);
        temp |= 0x8000000000;
    }
    else{
        temp = ((uint64_t)rb.get(0)<<32);
        temp |= 0x4000000000;
    }
    return temp;
}

enum class syncPatternType{
    MSB_NORMAL,
    MSB_INVERTED,
    LSB_NORMAL,
    LSB_INVERTED,
    COUNT
};