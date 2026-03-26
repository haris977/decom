#include "../include/frameWordFind.h"

int indexFind2ByteNoShift(uint8_t* buffer, int value, uint16_t wordPattern){
    uint8_t high = (wordPattern >> 8) & 0xFF;
    uint8_t low  = wordPattern & 0xFF;
    for (int i = 0; i <= value - 2; i ++){
        if (buffer[i] == high && buffer[i+1] == low){
            return i;
        }
    }
    return -1;
}

int indexFind2ByteWithShift(uint8_t* buffer, int value, uint16_t wordPattern, int offset){
    uint8_t high = (wordPattern >> 8) & 0xFF;
    uint8_t low  = wordPattern & 0xFF;
    for (int i = 0; i <= value - 3; i++){
        uint8_t first  = (buffer[i] << offset) | (buffer[i+1] >> (8-offset));
        uint8_t second = (buffer[i+1] << offset) | (buffer[i+2] >> (8-offset));
        if (first == high && second == low){
            return i;
        }
    }
    return -1;
}

int indexFind3ByteNoShift(uint8_t* buffer, int value, uint16_t wordPattern){
    uint8_t high = (wordPattern >> 16) & 0xFF;
    uint8_t low  = (wordPattern >> 8) & 0xFF;
    uint8_t third = wordPattern & 0xFF;
    for (int i = 0; i <= value - 3; i ++){
        if (buffer[i] == high && buffer[i+1] == low && buffer[i+2] == third){
            return i;
        }
    }
    return -1;
}

int indexFind3ByteWithShift(uint8_t* buffer, int value, uint16_t wordPattern, int offset){
    uint8_t sync_1 = (wordPattern >> 16) & 0xFF;
    uint8_t sync_2  = (wordPattern >>8) & 0xFF;
    uint8_t sync_3 = wordPattern & 0xFF;
    for (int i = 0; i <= value - 4; i++){
        uint8_t first  = (buffer[i] << offset) | (buffer[i+1] >> (8-offset));
        uint8_t second = (buffer[i+1] << offset) | (buffer[i+2] >> (8-offset));
        uint8_t third = (buffer[i+2] << offset) | (buffer[i+3] >> (8-offset));
        if (first == sync_1 && second == sync_2 && third == sync_3){
            return i;
        }
    }
    return -1;
}

int indexFind4ByteNoShift(uint8_t* buffer, int value, uint16_t wordPattern){
    uint8_t high = (wordPattern >> 24) & 0xFF;
    uint8_t low  = (wordPattern >> 16) & 0xFF;
    uint8_t third  = (wordPattern >>8) & 0xFF;
    uint8_t fourth = wordPattern & 0xFF;
    for (int i = 0; i <= value - 4; i ++){
        if (buffer[i] == high && buffer[i+1] == low && buffer[i+2] == third && buffer[i+3]== fourth){
            return i;
        }
    }
    return -1;
}

int indexFind4ByteWithShift(uint8_t* buffer, int value, uint16_t wordPattern, int offset){
    uint8_t sync_0 = (wordPattern >>24) & 0xFF;
    uint8_t sync_1 = (wordPattern >> 16) & 0xFF;
    uint8_t sync_2  = (wordPattern >>8) & 0xFF;
    uint8_t sync_3 = wordPattern & 0xFF;
    for (int i = 0; i <= value - 5; i++){
        uint8_t first  = (buffer[i] << offset) | (buffer[i+1] >> (8-offset));
        uint8_t second = (buffer[i+1] << offset) | (buffer[i+2] >> (8-offset));
        uint8_t third = (buffer[i+2] << offset) | (buffer[i+3] >> (8-offset));
        uint8_t fourth = (buffer[i+3] << offset) | (buffer[i+4] >> (8-offset));
        if (first == sync_0 && second == sync_1 && third == sync_2 && fourth == sync_3){
            return i;
        }
    }
    return -1;
}

int frameWordFind2Byte(uint8_t* buffer, int value, uint16_t wordPattern, int offset){
    if (offset == 0){
        return indexFind2ByteNoShift(buffer, value, wordPattern);
    }
    return indexFind2ByteWithShift(buffer, value, wordPattern, offset);
}

int frameWordFind3Byte(uint8_t* buffer, int value, uint32_t wordPattern, int offset){
    if (offset == 0){
        return indexFind3ByteNoShift(buffer, value, wordPattern);
    }
    return indexFind3ByteWithShift(buffer, value, wordPattern, offset);
}

int frameWordFind4Byte(uint8_t* buffer, int value, uint32_t wordPattern, int offset){
    if (offset == 0){
        return indexFind4ByteNoShift(buffer, value, wordPattern);
    }
    return indexFind4ByteWithShift(buffer, value, wordPattern, offset);
}