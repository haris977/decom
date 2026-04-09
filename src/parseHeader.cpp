#include "../include/ch10Header.h"
#include "../include/parseHeader.h"
#include "../include/getShiftedByte.h"
#include "../include/debugHeader.h"
uint32_t varifyPrimaryChecksum = 0;
uint16_t varifySecondaryChecksum = 0;
bool parsePrimaryHeader(RingBuffer& rb, int index,int counter, ch10PrimaryHeader& primaryHeader){
    // auto getByte = [&](int i){
    //     return getShiftedByteRingBuffer(rb,i+index,offset);
    // };
    varifyPrimaryChecksum = 0; // Reset checksum for new packet
    varifyPrimaryChecksum = checksumPrimaryHeader(rb);
    int start = rb.readindex.load();
    int end = (start+24)& rb.mask;
    
    primaryHeader.packetSyncWord = ((uint16_t)rb.get(0)<<8)&0xFFFF|(uint16_t)rb.get(1)&0xFFFF;
    primaryHeader.channelId = ((uint32_t)rb.get(2)<<8)|rb.get(3);
    primaryHeader.packetLength = (
        ((uint32_t)rb.get(4)<<24)|
        ((uint32_t)rb.get(5)<<16)|
        ((uint32_t)rb.get(6)<<8)|
        ((uint32_t)rb.get(7))
    );
    primaryHeader.dataLength = (
        ((uint32_t)rb.get(8)<<24)|
        ((uint32_t)rb.get(9)<<16)|
        ((uint32_t)rb.get(10)<<8)|
        ((uint32_t)rb.get(11))
    );
    primaryHeader.dataVersion    = rb.get(12);
    primaryHeader.sequenceNumber = rb.get(13);
    primaryHeader.packetFlag     = rb.get(14);
    primaryHeader.dataType       = rb.get(15);
    primaryHeader.RTCTime =
        ((uint64_t)rb.get(16) << 40) |
        ((uint64_t)rb.get(17) << 32) |
        ((uint64_t)rb.get(18) << 24) |
        ((uint64_t)rb.get(19) << 16) |
        ((uint64_t)rb.get(20) << 8) |
        ((uint64_t)rb.get(21) );
    primaryHeader.headerChecksum = (rb.get(22) << 8) | rb.get(23);
    counter += 24;
    // printf("parsed Header: sync of primaryheader : %04X , given checksum : %04X , calulated checksum : %04X\n",primaryHeader.packetSyncWord,primaryHeader.headerChecksum,varifyPrimaryChecksum);
    // printPrimaryHeader(primaryHeader);
    return varifyPrimaryChecksum==primaryHeader.headerChecksum;
}

bool parseSecondaryHeader(RingBuffer& rb, int index,int counter,ch10SecondaryHeader& secondaryHeader){
    auto getByte = [&](int i){
        return rb.get(index+i);
    };
    varifySecondaryChecksum = 0; // Reset checksum for new packet
    varifySecondaryChecksum = checksumSecondaryHeader(rb);
    // printf("parseheader: secondary: \n",rb.readindex);
    secondaryHeader.time = ((uint64_t)rb.get(index+0+24) << 56) |
        ((uint64_t)rb.get(index+1+24) << 48) |
        ((uint64_t)rb.get(index+2+24) << 40) |
        ((uint64_t)rb.get(index+3+24) << 32) |
        ((uint64_t)rb.get(index+4+24) << 24) |
        ((uint64_t)rb.get(index+5+24) << 16) |
        ((uint64_t)rb.get(index+6+24) << 8 ) |
        ((uint64_t)rb.get(index+7+24));
    // printf("%02X %02X %02X %02X %02X %02X %02X %02X : \n",rb.get(index+0+24),rb.get(index+1+24),rb.get(index+2+24),rb.get(index+3+24),rb.get(index+4+24),rb.get(index+5+24),rb.get(index+6+24),rb.get(index+7+24));    
    // for (int i = 0;i<40;i++){
    //     printf("%02X ",rb.get(index+i));
    // }
    // printf("\n");
    secondaryHeader.reserved = (((uint16_t)rb.get(index+8+24)<<8))|rb.get(index+24+9);
    secondaryHeader.secondayChecksum = (((uint16_t)rb.get(index+24+10)<<8))|rb.get(index+24+11);
    counter += 12;
    // printf("parseHeader secondary: calulated : %04X ,given : %04X : \n",varifySecondaryChecksum,secondaryHeader.secondayChecksum);
    return varifySecondaryChecksum==secondaryHeader.secondayChecksum;
}

bool parseChannelSpecifData(RingBuffer & rb, int index, channelSpecificData& channelSpecificData){
    channelSpecificData.value = ((uint32_t)rb.get(index)<<24)|
                                ((uint32_t)rb.get(index+1)<<16)|
                                ((uint32_t)rb.get(index+2)<<8) |
                                ((uint32_t)rb.get(index+3));
    return true;
}