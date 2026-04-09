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
#include "global.h"
#include "include/getShiftedByte.h"
#include "include/searching.h"
#include "include/syncFindWithMask.h"
#include "include/ch10Header.h"
#include "include/syncFindWithMask.h"
#include "include/getShiftedByte.h"
#include "include/debugHeader.h"
using namespace std;

// Define the global variable
int minorFrameIncrementer = -1;

int main()
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
#endif
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cerr << "socket() failed" << endl;
        return 1;
    }
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    // addr.sin_port = htons(60001);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
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
    cout << "Listening for UDP packets on port 9000..." << endl;
    uint8_t channelType[1] = {0xFF};
    int temp = 0;
    int status = 0; // 0 -> searching, 1->varify , 2->lock
    bool channel[1000] = {0};
    while (true)
    {
        ch10PrimaryHeader primaryHeader{};
        ch10SecondaryHeader secondaryHeader{};
        channelSpecificData channelSpecificData{};
        cout << endl
             << endl;
        temp++;
        uint8_t buffer[3200] = {0};
#ifdef _WIN32
        int len = sizeof(sender);
#else
        socklen_t len = sizeof(sender);
#endif
        int bytes = recvfrom(sockfd, (char *)buffer, 3200, 0,
                             (sockaddr *)&sender, &len);
        if (bytes < 0)
        {
            cerr << "recvfrom() failed" << endl;
            continue;
        }
        int syncOffset = 0;
        if (!rb.push(buffer, bytes) && temp == 1000)
        {
            temp = 0;
            cerr << " RingBuffer overflow while pushing packet data" << bytes << endl;
            continue;
        }
        if (rb.available() < 6400)
        {
            continue;
        }
        bool parsedHeader = false;
        int headerSize = rb.readindex.load();
        cout << "main_udp: read index start : " << rb.readindex.load() << endl;
        int variableIndex = searchingPacketSyncPattern(rb, primaryHeader, secondaryHeader, channelSpecificData);
        int syncIndex = (int)variableIndex & 0xFFFF;
        int parsedHeders = (variableIndex >> 16) & 0xFFFF;
        if (syncIndex >= 0)
        {
            int whole = 0;
            int prev = rb.writeindex.load();
            printf("before if and else : channel: %08X index %d \n", channelSpecificData.value, rb.readindex.load());
            if (channelSpecificData.value & 0x00001000){
                int dataLen = primaryHeader.dataLength;
                int counter = 0;
                while (counter < primaryHeader.dataLength){
                    int pos = findMinorSync32_auto(
                        rb,
                        0xFE6B2840,
                        counter,
                        primaryHeader,
                        secondaryHeader,
                        channelSpecificData);
                    if (pos == -1)
                    {
                        break; 
                    }
                    int bitPos = pos & 0xFFFF;
                    int method = (pos >> 16) & 0xFF; 
                    if (counter + 256 < primaryHeader.dataLength){
                        if (minorFrameIncrementer!=-1){
                            rb.advance(257-minorFrameIncrementer-1);
                            counter += 257-minorFrameIncrementer-1;
                            printf("main_udp: minorincrementor %d %d %d \n",rb.readindex.load(),counter,minorFrameIncrementer);
                            continue;
                        }
                        // cout<<"main_udp: if "<<counter+257<<" "<<rb.readindex<<endl;
                        printf("main_udp: if %d %d \n",rb.readindex.load(),counter);
                        rb.advance(257);
                        counter += 257;
                    }
                    else
                    {
                        int remaining = primaryHeader.dataLength - counter;
                        int rem = (int)primaryHeader.packetLength - rb.readindex.load()%3200;
                        std::cout << "main_udp: before else advance : "
                                  << rb.readindex.load()<<" "<<counter <<" "<<primaryHeader.dataLength<< std::endl;
                        rb.advance(remaining);
                        printPrimaryHeader(primaryHeader);
                        counter += remaining;
                        std::cout << "main_udp: after else advance: "
                                  << rb.readindex.load()
                                  << " counter: " << counter << std::endl;
                        break;
                    }
                }
                std::cout << "main_udp: after loop "
                          << rb.readindex.load() << std::endl;
                continue;
            }
            else
            {
                cout << "outside of throughput mode : " << endl;
                uint32_t offset = channelSpecificData.value & (0x3FF);
                if (channel[primaryHeader.channelId])
                {
                    // non bit shift
                }
                else
                {
                    // bitshift
                    uint8_t off = findMinorSync24_auto(rb, 0xFAF320);
                    for (int i = 0; i < 20; i++)
                    {
                        printf("02X", getShiftedByteRingBuffer(rb, i, off));
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
