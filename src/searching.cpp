#include <iostream>
#include "../include/searching.h"
int searchingPacketSyncPattern(RingBuffer& rb,ch10PrimaryHeader& primaryHeader,ch10SecondaryHeader& secondaryHeader,channelSpecificData& channel){
    int counter = 0;
    int syncOffset = 0;
    int syncIndex = findSyncPattern2ByteRingBuffer(rb,3200,0x25EB,syncOffset);
    
    if (syncIndex==-1){
        std::cout<<"searching: i get -1 in syncIndex: "<<std::endl;
        rb.advance(rb.available()-2);
        return -1;
    }
    bool parsedPrimaryHeader = parsePrimaryHeader(rb,syncIndex,counter,primaryHeader);
    if (!parsedPrimaryHeader){
        rb.advance(syncIndex+1);
        return -1;
    }    
    // rb.advance(24);
    bool parsedSeondaryHeader = false;
    
    if ((primaryHeader.packetFlag)==(0x82)){
        parsedSeondaryHeader = true;
        parseSecondaryHeader(rb,0,counter,secondaryHeader);
        std::cout<<"are we in seoncondaryHeader if: "<<parsedSeondaryHeader<<std::endl;
    }
    if (parsedSeondaryHeader){
        parseChannelSpecifData(rb,syncIndex+36,channel);
        rb.advance(40);
        printf("searching: inside of if: %d \n",rb.readindex.load());
    }
    else{
        parseChannelSpecifData(rb,syncIndex+24,channel);
        rb.advance(28);
        printf("searching: else: %d \n",rb.readindex.load());
    }
    if(parsedPrimaryHeader){
        syncIndex |= (syncIndex>>16)|0x010000 ;
    }
    if(parsedSeondaryHeader){
        syncIndex |= (syncIndex>>17)|0x020000;
    }
    
    return syncIndex;
}