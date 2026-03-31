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

void printPrimaryHeader(const ch10PrimaryHeader &h) {
    cout << hex << setfill('0');

    cout << "packetSyncWord : " << setw(4) << h.packetSyncWord << endl;
    cout << "channelId      : " << setw(4) << h.channelId << endl;
    cout << "packetLength   : " << setw(8) << h.packetLength << endl;
    cout << "dataLength     : " << setw(8) << h.dataLength << endl;
    cout << "dataVersion    : " << setw(2) << (int)h.dataVersion << endl;
    cout << "sequenceNumber : " << setw(2) << (int)h.sequenceNumber << endl;
    cout << "packetFlag     : " << setw(2) << (int)h.packetFlag << endl;
    cout << "dataType       : " << setw(2) << (int)h.dataType << endl;
    cout << "RTCTime        : " << setw(16) << h.RTCTime << endl;
    cout << "headerChecksum : " << setw(4) << h.headerChecksum << endl;

    cout << dec;
}

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
    // std::ofstream outfile("udp_receiver.csv", std::ios::trunc);
    // if (!outfile) {
    //     cerr << "Failed to open udp_receiver.csv" << endl;
    //     return 1;
    // }
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
        int counter = 0;
        // cout<<rb.writeindex<<endl;
        size_t syncIndex = searchingPacketSyncPattern(rb,syncOffset,parsedHeader,primaryHeader,secondaryHeader,channelSpecificData);
        if (syncIndex>=0){
            // printf("main_udp throughput check : %08X\n",channelSpecificData.value);
            // printPrimaryHeader(primaryHeader);
            // cout<<rb.writeindex<<endl;
            //20th bit is positive then we need to shift 12 bits to get 
            if (channelSpecificData.value & 0x00001000){
                // cout<<"main_udp: we are inside of syncIndex>0 "<<endl;
                //throughput mode.
                int pos = findMinorSync32_auto(rb,0xFE6B2840)%8;
                uint16_t bitPos = pos&0xFFFF;
                uint8_t method = (pos<<16)&0xFF;
                // cout<<"main_udp: readindex: "<<rb.readindex<<endl;
                // for (int i = 0;i<20;i++){
                //     printf("%02X",getShiftedByteRingBuffer(rb,i,off));
                // }
                // cout<<"main_udp: if of minorframesync: "<<endl;
                // printf("%d\n",off);
                // for (int i= 0;i<20;i++){
                //     printf("%02X ",getShiftedByteRingBuffer(rb,i,off));
                // }

                // printf("\n");
                rb.advance(255);
                continue;
            }
            else{
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
            rb.advance(255);
        }
        
        // if (status == 0) {
        //     syncIndex = searchingPacketSyncPattern(rb, syncOffset, parsedHeader, primaryHeader, secondaryHeader);
        //     cout<<"main udp this is syncIndex : "<<syncIndex<<endl;
        //     uint16_t pattern = ((uint16_t)rb.get(syncIndex+3200)<<8) | rb.get(syncIndex+3201);
        //     if (pattern == primaryHeader.packetSyncWord)
        //         status = 1;
        // }
        // if (status==1 || status==2) {
        //     uint16_t pattern = ((uint16_t)rb.get(syncIndex+3200)<<8) | rb.get(syncIndex+3201);  //what is this pattern ? print and see if possible?
        //     printf("this is next packet header: %04X\n",pattern);
        //     if (status == 2) {
        //         if (pattern != primaryHeader.packetSyncWord)
        //             status = 0;
        //         else {
        //             bool secondaryHeaderPresent = (rb.get(syncIndex+14)&1);
        //             int packetLength = primaryHeader.packetLength;
        //             rb.advance(packetLength);  // Remove processed packet from buffer
        //             uint8_t minorOffset = findMinorSync32_auto(rb,0xFE6B2840);
        //         }
        //     }
        //     if (status == 1) {
        //         if (pattern == primaryHeader.packetSyncWord)
        //             status = 2;
        //         else
        //             status = 0;
        //     }

        // }

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
