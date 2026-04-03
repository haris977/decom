#include "../include/debugHeader.h"
using namespace std;
void printPrimaryHeader(ch10PrimaryHeader &h) {
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