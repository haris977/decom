#pragma once
#include <cstdint>
#include "./ringBuffer.h"
#include <iostream>
#pragma pack(push,1)
struct ch10PrimaryHeader{
public:
    uint16_t packetSyncWord;
    uint16_t channelId;
    uint32_t packetLength;
    uint32_t dataLength;
    uint8_t dataVersion;
    uint8_t sequenceNumber;
    uint8_t packetFlag;
    uint8_t dataType;
    uint64_t RTCTime = 0;
    uint16_t headerChecksum;
};
#pragma pack(pop)

#pragma pack(push,1)
struct ch10SecondaryHeader{
    public:
    uint64_t time ;
    uint16_t reserved;
    uint16_t secondayChecksum;
};
#pragma pack(pop)

struct channelSpecificData{
    uint32_t value;
};

inline uint16_t fastChecksum24(const uint8_t* d) {
    for (int i = 0;i<22;i+=2){
        if (i==12 || i==14){
            uint16_t word = (((uint16_t)d[i]) | (uint16_t)d[i + 1] << 8)&0xFFFF;
            printf("%04X ",word);
            continue;
        }
        uint16_t word = (((uint16_t)d[i] << 8) | (uint16_t)d[i + 1])&0xFFFF;
        printf("%04X ",word);
    }
    printf("\n");
    uint32_t sum =
        ((uint16_t)d[0]<<8 | ((uint16_t)d[1])) +
        ((uint16_t)d[2]<<8 | ((uint16_t)d[3])) +
        ((uint16_t)d[4]<<8 | ((uint16_t)d[5])) +
        ((uint16_t)d[6]<<8 | ((uint16_t)d[7])) +
        ((uint16_t)d[8]<<8 | ((uint16_t)d[9])) +
        ((uint16_t)d[10]<<8 | ((uint16_t)d[11])) +
        ((uint16_t)d[12] | ((uint16_t)d[13]<<8)) +
        ((uint16_t)d[14] | ((uint16_t)d[15]<<8)) +
        ((uint16_t)d[16]<<8 | ((uint16_t)d[17])) +
        ((uint16_t)d[18]<<8 | ((uint16_t)d[19])) +
        ((uint16_t)d[20]<<8 | ((uint16_t)d[21]));

    sum = (sum & 0xFFFF) + (sum >> 16);
    sum = (sum & 0xFFFF) + (sum >> 16);

    return sum;
}
inline uint16_t checksumPrimaryHeader(RingBuffer& rb){
    int start = rb.readindex.load();

    if (start+24<=RingBuffer::size){
        uint8_t * ptr = &rb.buffer[start];
        return fastChecksum24(ptr);
    }
    uint8_t temp[24];
    int firstPart = RingBuffer::size - start;
    memcpy(temp,&rb.buffer[start],firstPart);
    memcpy(temp+firstPart,rb.buffer,24-firstPart);
    return fastChecksum24(temp);
}

inline uint16_t fastChecksum12(const uint8_t* d) {
    uint32_t sum =
        ((uint16_t)d[0] | (d[1]<<8))  +
        ((uint16_t)d[2] | (d[3]<<8))  +
        ((uint16_t)d[4] | (d[5]<<8))  +
        ((uint16_t)d[6] | (d[7]<<8))  +
        ((uint16_t)d[8] | (d[9]<<8));
    // printf("ch10header.h : %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X sum = %08X : \n",d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9],sum);
    sum = (sum & 0xFFFF) + (sum >> 16);
    sum = (sum & 0xFFFF) + (sum >> 16);
    // printf("sum = %04X \n",sum);
    return (uint16_t)sum;
}

inline uint16_t checksumSecondaryHeader(RingBuffer& rb){
    int start = rb.readindex.load()+24;
    if (start+12<=RingBuffer::size){
        return fastChecksum12(&rb.buffer[start]);
    }
    uint8_t temp[12];
    int first = RingBuffer::size - start;
    memcpy(temp,&rb.buffer[start],first);
    memcpy(temp+first,&rb.buffer,12-first);
    return fastChecksum12(temp);
}