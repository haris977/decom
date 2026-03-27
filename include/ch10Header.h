#pragma once
#include <cstdint>
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
    uint64_t RTCTime;
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