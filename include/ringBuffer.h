#pragma once 
#include <cstdint>
#include <iostream>
class RingBuffer{
    int tt = 0;
public:
    static constexpr int size = 65536;
    static constexpr int mask = size - 1;
    uint8_t buffer[size];
    int readindex = 0;
    int writeindex = 0;
    int count = 0;
    bool push(uint8_t* data, int len){
        tt++;
        if (len>size-count) {
            if (tt==100){
                tt = 0;
                printf("ringBuffer : length : %d, size: %d, count: %d\n",len,size,count);
            }
            return false;
        }
        for (int i = 0;i<len;i++){
            buffer[writeindex] = data[i];
            writeindex = (writeindex +1)&mask;
        }    
        count += len;
        return true;
    }

    inline uint8_t get(int index){
        return buffer[(readindex+index)&mask];
    }

    // inline uint8_t shiftBit(int index, int offset){
    //     uint8_t left = (get(index)<<offset)&0xFF;
    //     uint8_t right = (get(index+1)>>(8-offset))&0xFF;
    //     return left|right;
    // }

    void advance(int n){
        readindex = (readindex+n)&mask;
        count -= n;
    }

    int available(){
        return count;
    }
};

