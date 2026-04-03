#include "ringBuffer.h"
#include "packetSyncFind.h"
#include "parseHeader.h"
#include "ch10Header.h"
int searchingPacketSyncPattern(RingBuffer& rb,int offset,bool parseHeader,ch10PrimaryHeader & ch10PrimaryHeader,ch10SecondaryHeader& secondaryHeader,channelSpecificData& channel);