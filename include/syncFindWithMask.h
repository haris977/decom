#pragma once
#include <cstdint>
#include "ringBuffer.h"
uint32_t reverse32(uint32_t x);
uint32_t reverse24(uint32_t x);
uint16_t reverse16(uint16_t x);
void parseStream(RingBuffer& rb);
int findMinorSync32_rb(RingBuffer& rb, uint32_t sync);
int findMinorSync32_auto(RingBuffer& rb, uint32_t sync);
enum class syncPatternType{
    MSB_NORMAL,
    MSB_INVERTED,
    LSB_NORMAL,
    LSB_INVERTED,
    COUNT
};