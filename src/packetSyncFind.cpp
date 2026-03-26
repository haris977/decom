#include "../include/packetSyncFind.h"
#include "../include/ringBuffer.h"
int findSyncPattern2Byte(uint8_t* buffer, int value , uint16_t syncPattern,int& offset){
    for(int i = 0;i<value-3;i++){
        if (buffer[i]==(syncPattern>>8)&0xFF && buffer[i+1]==syncPattern&0xFF){
            return i;
        }
        else{
            for (int shift = 1;shift<=7;shift++){
                uint8_t first = (buffer[i]<<shift)|((buffer[i+1]>>(8-shift))&0xFF);
                uint8_t second = (buffer[i+1]<<shift)|((buffer[i+2]>>(8-shift))&0xFF);
                if ((first== syncPattern>>8)&0xFF && second == syncPattern&0xFF){
                    offset = shift;
                    return i;
                }
            }
        }
    }
    if (buffer[value-2]==(syncPattern>>8)&0xFF && buffer[value-1]==syncPattern&0xFF){
        return value-2;
    }
    return -1;
}

int findSyncPattern3Byte(uint8_t* buffer, int value , uint16_t syncPattern,int& offset){
    for(int i = 0;i<value-4;i++){
        if (buffer[i]==(syncPattern>>16)&0xFF && buffer[i+1]==(syncPattern>>8)&0xFF && buffer[i+1]==syncPattern&0xFF){
            return i;
        }
        else{
            for (int shift = 1;shift<=7;shift++){
                uint8_t first = (buffer[i]<<shift)|((buffer[i+1]>>(8-shift))&0xFF);
                uint8_t second = (buffer[i+1]<<shift)|((buffer[i+2]>>(8-shift))&0xFF);
                uint8_t third = (buffer[i+1]<<shift)|((buffer[i+2]>>(8-shift))&0xFF);
                if ((first== syncPattern>>16)&0xFF && second ==(syncPattern>>8)&0xFF && third == syncPattern&&0xFF){
                    offset = shift;
                    return i;
                }
            }
        }
    }
    if (buffer[value-3]==(syncPattern>>16)&0xFF && buffer[value-2]==(syncPattern>>8)&0xFF && buffer[value-1]==syncPattern&0xFF){
        return value-3;
    }
    return -1;
}

int findSyncPattern4Byte(uint8_t* buffer, int value , uint32_t syncPattern,int& offset){
    for(int i = 0;i<value-5;i++){
        if (buffer[i]==(syncPattern>>24)&0xFF && buffer[i+1]==(syncPattern>>16)&0xFF && buffer[i+2]==(syncPattern>>8)&0xFF && buffer[i+3]==syncPattern&0xFF){
            return i;
        }
        else{
            for (int shift = 1;shift<=7;shift++){
                uint8_t first = (buffer[i]<<shift)|((buffer[i+1]>>(8-shift))&0xFF);
                uint8_t seconds = (buffer[i+1]<<shift)|((buffer[i+2]>>(8-shift))&0xFF);
                uint8_t third = (buffer[i+2]<<shift)|((buffer[i+3]>>(8-shift))&0xFF);
                uint8_t fourth = (buffer[i+3]<<shift)|((buffer[i+4]>>(8-shift))&0xFF);
                if ((first== syncPattern>>8)&0xFF && syncPattern&0xFF){
                    offset = shift;
                    return i;
                }
            }
        }
    }
    if (buffer[value-4]==(syncPattern>>24)&0xFF && buffer[value-3]==(syncPattern>>16)&0xFF &&buffer[value-2]==(syncPattern>>8)&0xFF && buffer[value-1]==syncPattern&0xFF){
        return value-4;
    }
    return -1;
}

/********---------------------packet sync find with ring buffer------------------------*********/
int findSyncPattern2ByteRingBuffer(RingBuffer & rb, int value, uint16_t syncPattern, int& offset){
    int n = rb.available();
    if (n<3){
        return -1;
    }
    uint8_t high = (syncPattern>>8)&0xFF;
    uint8_t low = syncPattern&0xFF;
    for (int i = 0;i<n-1;i++){
        if (rb.get(i)==high && rb.get(i+1)==low){
            offset = 0;
            return i;
        }
        // for (int shift=1;shift<=7;shift++){
        //     uint8_t first = ((rb.get(i)<<shift)&0xFF)|((rb.get(i+1)>>(8-shift))&0xFF);
        //     uint8_t second = ((rb.get(i+1)<<shift)&0xFF) | ((rb.get(i+2)>>(8-shift))&0xFF);
        //     if (first==high && second == low){
        //         offset = shift;
        //         return i;
        //     }
        // }
    }
    // if (rb.get(n-2)==high && rb.get(n-1)==low){
    //     return n-2;
    // }
    return -1;
}
