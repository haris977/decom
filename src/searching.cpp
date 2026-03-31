#include <iostream>
#include "../include/searching.h"
size_t searchingPacketSyncPattern(RingBuffer& rb, int syncOffset,bool parsedPrimaryHeader,ch10PrimaryHeader& primaryHeader,ch10SecondaryHeader& secondaryHeader,channelSpecificData& channel){
    int counter = 0;
    size_t syncIndex = -1;
    syncIndex = findSyncPattern2ByteRingBuffer(rb,3200,0x25EB,syncOffset);
    if (syncIndex==-1){
        rb.advance(rb.available()-2);
    }
    parsedPrimaryHeader = parsePrimaryHeader(rb,syncIndex,counter,primaryHeader);
    if (!parsedPrimaryHeader) return -1;
    bool parsedSeondaryHeader = false;
    
    if (primaryHeader.packetFlag&1){
        parsedSeondaryHeader = parseSecondaryHeader(rb,syncIndex+24,counter,secondaryHeader);
    }
    if (parsedSeondaryHeader){
        parseChannelSpecifData(rb,syncIndex+36,channel);
        rb.advance(35);
    }
    else{
        parseChannelSpecifData(rb,syncIndex+24,channel);
        rb.advance(27);
    }
    // rb.advance(syncIndex+counter);
    
    // if (syncIndex!=-1){
    //     printf(" Index : %d,writeIndex: %d, ReadIndex: %d,AvaliableSize: %d, seconds : %02X, third : %02X,fourth: %02X\n",syncIndex,rb.writeindex,rb.readindex,rb.available(),rb.get(3200+syncIndex),rb.get(3200+syncIndex+1),rb.get(3200+syncIndex+2));
    // }
    return syncIndex;
}