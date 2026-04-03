#include <iostream>
#include "../include/searching.h"
int searchingPacketSyncPattern(RingBuffer& rb, int syncOffset,bool parsedPrimaryHeader,ch10PrimaryHeader& primaryHeader,ch10SecondaryHeader& secondaryHeader,channelSpecificData& channel){
    int counter = 0;
    int syncIndex = findSyncPattern2ByteRingBuffer(rb,3200,0x25EB,syncOffset);
    
    if (syncIndex==-1){
        std::cout<<"searching: i get -1 in syncIndex: "<<std::endl;
        rb.advance(rb.available()-2);
        return -1;
    }
    parsedPrimaryHeader = parsePrimaryHeader(rb,syncIndex,counter,primaryHeader);
    if (!parsedPrimaryHeader){
        rb.advance(syncIndex+1);
        return -1;
    }    
    bool parsedSeondaryHeader = false;
    
    if (primaryHeader.packetFlag&1){
        parsedSeondaryHeader = parseSecondaryHeader(rb,syncIndex+24,counter,secondaryHeader);
    }
    if (parsedSeondaryHeader){
        parseChannelSpecifData(rb,syncIndex+36,channel);
        printf("searching: inside of if: %08X",channel.value);
        rb.advance(36);
    }
    else{
        parseChannelSpecifData(rb,syncIndex+24,channel);
        printf("searching: inside of else: %08X",channel.value);
        rb.advance(28);
    }
    return syncIndex;
}