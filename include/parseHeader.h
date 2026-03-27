#pragma once
#include <cstdint>
#include "../include/ringBuffer.h"
#include "./ch10Header.h"
bool parsePrimaryHeader(RingBuffer& rb, int index,int counter,ch10PrimaryHeader & ch10PrimaryHeader );
bool parseSecondaryHeader(RingBuffer& rb, int index,int counter,ch10SecondaryHeader & ch10SecondaryHeader );
bool parseChannelSpecifData(RingBuffer & rb, int index, channelSpecificData & channelSpecificData);