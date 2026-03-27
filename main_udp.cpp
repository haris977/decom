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

        size_t syncIndex = searchingPacketSyncPattern(rb,syncOffset,parsedHeader,primaryHeader,secondaryHeader);
        if (syncIndex>=0){
            if (channelSpecificData.value>>19 & 1){
                //throughput mode.
            }
            else{
                uint32_t offset = channelSpecificData.value&(0x3FF);
                if(channel[primaryHeader.channelId]){
                    //non bit shift
                    
                }
                else{
                    //bitshift
                }
            }
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
