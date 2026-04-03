#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <vector>
#include "include/getShiftedByte.h"
#include "include/searching.h"
#include "include/syncFindWithMask.h"
#include "include/ch10Header.h"
#include "include/syncFindWithMask.h"
#include "include/getShiftedByte.h"
using namespace std;

int main(){
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
#endif
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "socket() failed" << endl;
        return 1;
    }
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    // addr.sin_port = htons(60001);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        cerr << "bind() failed on UDP port 9000" << endl;
        return 1;
    }
    sockaddr_in sender;
#ifdef _WIN32
    int len = sizeof(sender);
#else
    socklen_t len = sizeof(sender);
#endif
    RingBuffer rb;
    ch10PrimaryHeader primaryHeader{};
    ch10SecondaryHeader secondaryHeader{};
    channelSpecificData channelSpecificData{};
    cout << "Listening for UDP packets on port 9000..." << endl;
    uint8_t channelType[1] = {0xFF};
    int temp = 0;
    int status = 0; // 0 -> searching, 1->varify , 2->lock
    bool channel[1000] = {0};
    while (true){
        temp ++;
        uint8_t buffer[3200] = {0};
#ifdef _WIN32 
        int len = sizeof(sender);
#else
        socklen_t len = sizeof(sender);
#endif
        int bytes = recvfrom(sockfd, (char*)buffer, 3200, 0,
                             (sockaddr *)&sender, &len);
        if (bytes < 0) {
            cerr << "recvfrom() failed" << endl;
            continue;
        }
        int syncOffset = 0;
        if (!rb.push(buffer, bytes) && temp == 1000) {
            temp = 0;
            cerr << " RingBuffer overflow while pushing packet data"<< bytes << endl;
            continue;
        }
        if (rb.available()<6400){
            continue;
        }
        bool parsedHeader = false;
        int headerSize = rb.readindex;
        int syncIndex = searchingPacketSyncPattern(rb,syncOffset,parsedHeader,primaryHeader,secondaryHeader,channelSpecificData);
       
        if (syncIndex==123456789){
            cout<<"main_udp: we should not be here : "<<endl;
            break;
        }
        if (syncIndex>=0){
            int whole = 0;
            int prev = rb.writeindex;
            printf("channel: %08X\n",channelSpecificData.value);
            if (channelSpecificData.value & 0x00001000){
                int pos = findMinorSync32_auto(rb,0xFE6B2840);
                int bitPos = pos&0xFFFF;
                int method = (pos<<16)&0xFF;
                int counter = bitPos/8+headerSize;
                while (counter<(int)primaryHeader.dataLength){
                    if (counter+255<(int)primaryHeader.dataLength){
                        rb.advance(257);
                        counter += 257;
                        
                    }
                    else{
                        rb.advance((int)primaryHeader.packetLength-counter);
                        counter += (int)primaryHeader.dataLength-counter;
                        break;
                    }
                    pos = findMinorSync32_auto(rb,0xFE6B2840);
                    if (pos==-1){
                        break;
                    }
                }
               
                continue;
            }
            else{
                cout<<"outside of throughput mode : "<<endl;
                uint32_t offset = channelSpecificData.value&(0x3FF);
                if(channel[primaryHeader.channelId]){
                    //non bit shift
                    
                }
                else{
                    //bitshift
                    uint8_t off = findMinorSync24_auto(rb,0xFAF320);
                    for (int i = 0;i<20;i++){
                        printf("02X",getShiftedByteRingBuffer(rb,i,off));
                    }
                    rb.advance(255);
                }
                continue;
            }
        }

    }
    // outfile.close();
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return 0;
}
