#include <cstdint>
#include <cstdint>
#include "./ringBuffer.h"
int findSyncPattern2Byte(uint8_t* buffer,int value,uint16_t syncPattern,int& offset );
int findSyncPattern3Byte(uint8_t* buffer,int value,uint32_t syncPattern,int& offset );
int findSyncPattern4Byte(uint8_t* buffer, int value, uint32_t syncPattern, int& offset );
int findSyncPattern2ByteRingBuffer(RingBuffer &rb, int value, uint16_t syncPattern,int& offset);
int findSyncPattern3ByteRingBuffer(RingBuffer &rb, int value, uint16_t syncPattern,int& offset);
int findSyncPattern4ByteRingBuffer(RingBuffer &rb, int value, uint16_t syncPattern,int& offset);
