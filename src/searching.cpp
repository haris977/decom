#include <iostream>
#include "../include/searching.h"
size_t searchingPacketSyncPattern(RingBuffer& rb, int syncOffset,bool parsedHeader,ch10PrimaryHeader& primaryHeader,ch10SecondaryHeader& secondaryHeader){
    int counter = 0;
    size_t syncIndex = -1;
        syncIndex = findSyncPattern2ByteRingBuffer(rb,3200,0xEB25,syncOffset);
        if (syncIndex==-1){
            rb.advance(rb.available()-2);
        }
        parsePrimaryHeader(rb,syncIndex,counter,syncOffset,primaryHeader);
        if (primaryHeader.packetFlag&1){
            parseSecondaryHeader(rb,syncIndex+24,counter,secondaryHeader);
        }
        // rb.advance(syncIndex+counter);
        parsedHeader = true;
        if (syncIndex!=-1){
            printf(" Index : %d,writeIndex: %d, ReadIndex: %d,AvaliableSize: %d, seconds : %02X, third : %02X,fourth: %02X\n",syncIndex,rb.writeindex,rb.readindex,rb.available(),rb.get(3200+syncIndex),rb.get(3200+syncIndex+1),rb.get(3200+syncIndex+2));
        }
    return syncIndex;
}