#pragma once 
#include <cstdint>
#include "./ringBuffer.h"
uint8_t getShiftedByte(uint8_t* buffer, int pos,int value, int offset);
uint8_t getShiftedByteRingBuffer(RingBuffer& rb, int pos, int offset);