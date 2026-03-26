#include "../include/getShiftedByte.h"
uint8_t getShiftedByte(uint8_t* buffer, int index,int value, int offset){
    if(offset==0) return buffer[index];
    uint8_t next = 0;
    if (index+1<value){
        next = buffer[index+1];
    }
    return ((buffer[index]<<offset)&0xFF)|((next>>(8-offset))&0xFF);
}
uint8_t getShiftedByteRingBuffer(RingBuffer& rb, int pos, int offset){
    uint8_t curr = rb.get(pos);
    if (offset==0){
        return curr;
    }
    uint8_t next = rb.get(pos+1);
    return ((curr<<offset)&0xFF) | ((next>>(8-offset))&0xFF);
}
