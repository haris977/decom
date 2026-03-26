#pragma once 
#include <cstdint>
class RingBuffer{
public:
    static constexpr int size = 65536;
    static constexpr int mask = size -1;

    uint8_t buffer[size];
    int readindex = 0;
    int writeindex = 0;
    int count = 0;
    bool push(uint8_t* data, int len){
        if (len>size-count) return false;
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
    void advance(int n){
        readindex = (readindex+n)&mask;
        count -= n;
    }
    int available(){
        return count;
    }
};