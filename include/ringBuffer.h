#pragma once 
#include <cstdint>
#include <atomic>
#include <iostream>
class RingBuffer{
    int tt = 0;
public:
    static constexpr int size = 65536;
    static constexpr int mask = size -1;
    uint8_t buffer[size];
    std::atomic<int> readindex{0};
    std::atomic<int> writeindex{0};
private:
    int lastReadindex = 0;
    int lastWriteindex = 0;
public:
    bool push(uint8_t * data, int len){
        int currentWrite = writeindex.load(std::memory_order_acquire);
        int currentRead = readindex.load(std::memory_order_acquire);  // FIXED: was writeindex (bug!)
        int count = (currentWrite - currentRead + size)&mask;
        if (len>size-count){
            printf("rinbuffer: length %d , size: %d , avaliable: %d \n",len,size,size-count);
            return false;
        }
        for (int i = 0;i<len;i++){
            buffer[(currentWrite+i)&mask] = data[i];
        }

        //realease write so reader can see new data
        writeindex.store((currentWrite + len)&mask,std::memory_order_release);
        return true;
    }
    inline uint8_t get(int index){
        return buffer[(readindex.load(std::memory_order_acquire)+index)& mask];
    }
    void advance (int n ){
        int currentRead = readindex.load(std::memory_order_acquire);
        readindex.store((currentRead + n)&mask,std::memory_order_release);
    }
    int available(){
        int currentWrite = writeindex.load(std::memory_order_acquire);
        int currentRead = readindex.load(std::memory_order_acquire);
        return (currentWrite-currentRead+size)&mask;
    }
};

// #pragma once 
// #include <cstdint>
// #include <iostream>
// class RingBuffer{
//     int tt = 0;
// public:
//     static constexpr int size = 65536;
//     static constexpr int mask = size - 1;
//     uint8_t buffer[size];
//     int readindex = 0;
//     int writeindex = 0;
//     int count = 0;
//     bool push(uint8_t* data, int len){
//         tt++;
//         if (len>size-count) {
//             if (tt==100){
//                 tt = 0;
//                 printf("ringBuffer : length : %d, size: %d, count: %d\n",len,size,count);
//             }
//             return false;
//         }
//         for (int i = 0;i<len;i++){
//             buffer[writeindex] = data[i];
//             writeindex = (writeindex +1)&mask;
//         }    
//         count += len;
//         return true;
//     }

//     inline uint8_t get(int index){
//         return buffer[(readindex+index)&mask];
//     }

//     // inline uint8_t shiftBit(int index, int offset){
//     //     uint8_t left = (get(index)<<offset)&0xFF;
//     //     uint8_t right = (get(index+1)>>(8-offset))&0xFF;
//     //     return left|right;
//     // }

//     void advance(int n){
//         readindex = (readindex+n)&mask;
//         count -= n;
//     }

//     int available(){
//         return count;
//     }
// };

