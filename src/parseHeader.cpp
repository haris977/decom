#include "../include/ch10Header.h"
#include "../include/parseHeader.h"
#include "../include/getShiftedByte.h"
uint16_t varifyPrimaryChecksum = 0;
uint16_t varifySecondaryChecksum = 0;
bool parsePrimaryHeader(RingBuffer& rb, int index,int counter, ch10PrimaryHeader& primaryHeader){
    // auto getByte = [&](int i){
    //     return getShiftedByteRingBuffer(rb,i+index,offset);
    // };
    primaryHeader.packetSyncWord = ((uint16_t)rb.get(0)<<8)|rb.get(1);
    varifyPrimaryChecksum = (varifyPrimaryChecksum + primaryHeader.packetSyncWord)&0xFF;
    primaryHeader.channelId = ((uint32_t)rb.get(2)<<8)|rb.get(3);
    varifyPrimaryChecksum = (varifyPrimaryChecksum + primaryHeader.channelId)&0xFF;
    primaryHeader.packetLength = (
        ((uint32_t)rb.get(4)<<24)|
        ((uint32_t)rb.get(5)<<16)|
        ((uint32_t)rb.get(6)<<8)|
        ((uint32_t)rb.get(7))
    );
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (primaryHeader.packetLength>>16)&0xFF + (primaryHeader.packetLength)&0xFF)&0xFF;
    primaryHeader.dataLength = (
        ((uint32_t)rb.get(8)<<24)|
        ((uint32_t)rb.get(9)<<16)|
        ((uint32_t)rb.get(10)<<8)|
        ((uint32_t)rb.get(11))
    );
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (primaryHeader.dataLength>>16)&0xFF + (primaryHeader.dataLength)&0xFF)&0xFF;
    primaryHeader.dataVersion    = rb.get(12);
    primaryHeader.sequenceNumber = rb.get(13);
    primaryHeader.packetFlag     = rb.get(14);
    primaryHeader.dataType       = rb.get(15);
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (uint16_t)primaryHeader.dataVersion)&0xFF;
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (uint16_t)primaryHeader.sequenceNumber)&0xFF;
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (uint16_t)primaryHeader.packetFlag)&0xFF;
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (uint16_t)primaryHeader.dataType)&0xFF;
    primaryHeader.RTCTime =
        ((uint64_t)rb.get(16) << 56) |
        ((uint64_t)rb.get(17) << 48) |
        ((uint64_t)rb.get(18) << 40) |
        ((uint64_t)rb.get(19) << 32) |
        ((uint64_t)rb.get(20) << 24) |
        ((uint64_t)rb.get(21) << 16) |
        ((uint64_t)rb.get(22) << 8) |
        rb.get(23);
    varifyPrimaryChecksum = (varifyPrimaryChecksum + (primaryHeader.RTCTime>>56)&0xFF + (primaryHeader.RTCTime>>48)&0xFF + (primaryHeader.RTCTime>>40)&0xFF  + (primaryHeader.RTCTime>>32)&0xFF + (primaryHeader.RTCTime>>24)&0xFF + (primaryHeader.RTCTime>>16)&0xFF + (primaryHeader.RTCTime>>8)&0xFF+ (primaryHeader.RTCTime)&0xFF)&0xFF;
    primaryHeader.headerChecksum = (rb.get(24) << 8) | rb.get(25);
    counter += 24;
    return varifyPrimaryChecksum==primaryHeader.headerChecksum;
}

bool parseSecondaryHeader(RingBuffer& rb, int index,int counter,ch10SecondaryHeader& secondaryHeader){
    auto getByte = [&](int i){
        return rb.get(index+i);
    };
    secondaryHeader.time = ((uint64_t)rb.get(16) << 56) |
        ((uint64_t)rb.get(0) << 56) |
        ((uint64_t)rb.get(1) << 48) |
        ((uint64_t)rb.get(2) << 40) |
        ((uint64_t)rb.get(3) << 32) |
        ((uint64_t)rb.get(4) << 24) |
        ((uint64_t)rb.get(5) << 16) |
        ((uint64_t)rb.get(6) << 8 ) |
        ((uint64_t)rb.get(7));
    varifySecondaryChecksum = (varifySecondaryChecksum + (secondaryHeader.time>>56)&0xFF + (secondaryHeader.time>>48)&0xFF + (secondaryHeader.time>>40)&0xFF  + (secondaryHeader.time>>32)&0xFF + (secondaryHeader.time>>24)&0xFF + (secondaryHeader.time>>16)&0xFF + (secondaryHeader.time>>8)&0xFF+ (secondaryHeader.time)&0xFF)&0xFF;
    secondaryHeader.reserved = (((uint16_t)rb.get(8)<<8))|rb.get(9);
    varifySecondaryChecksum = (varifySecondaryChecksum + secondaryHeader.reserved)&0xFF;
    secondaryHeader.secondayChecksum = (((uint16_t)rb.get(10)<<8))|rb.get(11);
    counter += 12;
    return varifySecondaryChecksum==secondaryHeader.secondayChecksum;
}

bool parseChannelSpecifData(RingBuffer & rb, int index, channelSpecificData& channelSpecificData){
    channelSpecificData.value = ((uint32_t)rb.get(index)<<24)|
                                ((uint32_t)rb.get(index+1)<<16)|
                                ((uint32_t)rb.get(index+2)<<8) |
                                ((uint32_t)rb.get(index+3));
    return true;
}