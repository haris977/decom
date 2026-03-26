#include "../include/ch10Header.h"
#include "../include/parseHeader.h"
#include "../include/getShiftedByte.h"
void parsePrimaryHeader(RingBuffer& rb, int index,int counter, int offset, ch10PrimaryHeader& primaryHeader){
    auto getByte = [&](int i){
        return getShiftedByteRingBuffer(rb,i+index,offset);
    };
    primaryHeader.packetSyncWord = (getByte(0)<<8)|getByte(1);
    primaryHeader.channelId = (getByte(2)<<8)|getByte(3);
    primaryHeader.packetLength = (
        ((uint32_t)getByte(4)<<24)|
        ((uint32_t)getByte(5)<<16)|
        ((uint32_t)getByte(6)<<8)|
        ((uint32_t)getByte(7))
    );
    primaryHeader.dataLength = (
        ((uint32_t)getByte(8)<<24)|
        ((uint32_t)getByte(9)<<16)|
        ((uint32_t)getByte(10)<<8)|
        ((uint32_t)getByte(11))
    );
    primaryHeader.dataVersion    = getByte(12);
    primaryHeader.sequenceNumber = getByte(13);
    primaryHeader.packetFlag     = getByte(14);
    primaryHeader.dataType       = getByte(15);

    primaryHeader.RTCTime =
        ((uint64_t)getByte(16) << 56) |
        ((uint64_t)getByte(17) << 48) |
        ((uint64_t)getByte(18) << 40) |
        ((uint64_t)getByte(19) << 32) |
        ((uint64_t)getByte(20) << 24) |
        ((uint64_t)getByte(21) << 16) |
        ((uint64_t)getByte(22) << 8) |
        getByte(23);
    primaryHeader.headerChecksum = (getByte(24) << 8) | getByte(25);
    counter += 24;
}

void parseSecondaryHeader(RingBuffer& rb, int index,int counter,ch10SecondaryHeader& secondaryHeader){
    auto getByte = [&](int i){
        return rb.get(index+i);
    };
    secondaryHeader.time = ((uint64_t)getByte(16) << 56) |
        ((uint64_t)getByte(0) << 56) |
        ((uint64_t)getByte(1) << 48) |
        ((uint64_t)getByte(2) << 40) |
        ((uint64_t)getByte(3) << 32) |
        ((uint64_t)getByte(4) << 24) |
        ((uint64_t)getByte(5) << 16) |
        ((uint64_t)getByte(6) << 8 ) |
        ((uint64_t)getByte(7));
    secondaryHeader.reserved = (((uint16_t)getByte(8)<<8))|getByte(9);
    secondaryHeader.secondayChecksum = (((uint16_t)getByte(10)<<8))|getByte(11);
    counter += 12;
}
