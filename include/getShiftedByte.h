#pragma once 
#include <cstdint>
#include "./ringBuffer.h"
inline uint8_t getShiftedByte(uint8_t* buffer, int pos,int value, int offset){
    if(offset==0) return buffer[pos];
    uint8_t next = 0;
    if (pos+1<value){
        next = buffer[pos+1];
    }
    return ((buffer[pos]<<offset)&0xFF)|((next>>(8-offset))&0xFF);
}
inline uint8_t getShiftedByteRingBuffer(RingBuffer& rb, int pos, int offset){
    uint8_t curr = rb.get(pos);
    if (offset==0){
        return curr;
    }
    uint8_t next = rb.get(pos+1);
    return ((curr<<offset)&0xFF) | ((next>>(8-offset))&0xFF);
}