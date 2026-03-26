#pragma once
#include <cstdint>
#include "../include/ringBuffer.h"
#include "./ch10Header.h"
void parsePrimaryHeader(RingBuffer& rb, int index,int counter, int offset,ch10PrimaryHeader & ch10PrimaryHeader );
void parseSecondaryHeader(RingBuffer& rb, int index,int counter,ch10SecondaryHeader & ch10SecondaryHeader );