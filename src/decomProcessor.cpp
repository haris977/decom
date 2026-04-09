// #include "../include/decomProcessor.h"
// #include <iostream>
// #include <cstring>

// using namespace std;

// DecomProcessor::DecomProcessor(size_t bufferSize)
//     : ctx(rb), packetBuffer(bufferSize)
// {
//     cout << "[DecomProcessor] Initialized with buffer size: " << bufferSize << endl;
// }

// DecomProcessor::~DecomProcessor() {
//     cout << "[DecomProcessor] Shutting down, total packets: " << ctx.packetCount << endl;
// }

// // ✅ NOTICE: Functions take NO PARAMETERS - ONLY use ctx and member variables
// bool DecomProcessor::processPacket(uint8_t* data, size_t length) {
//     if (!data || length == 0) return false;
    
//     ctx.buffer = data;
//     ctx.bufferLength = length;
//     ctx.readIndex = 0;
    
//     // if (ctx.enableDebug) {
//     //     logDebug("Processing packet of size: " + to_string(length));
//     // }
    
//     // Process through ring buffer
//     if (!rb.push(data, length)) {
//         cerr << "[ERROR] Ring buffer overflow" << endl;
//         return false;
//     }
    
//     if (ctx.status == 0) { // Searching
//         return findSync(0xEB25);  // Example sync word
//     } else if (ctx.status == 1) { // Verify
//         return parseHeaders();
//     }
    
//     return true;
// }

// //✅ findSync accesses ctx directly - NO parameters needed for context data

// bool DecomProcessor::findSync(uint32_t syncWord) {
//     // Access all data through ctx - NO function parameters!
//     while (ctx.rb.available() >= 5) {
//         uint64_t temp = ((uint64_t)ctx.rb.get(0) << 32) |
//                         ((uint64_t)ctx.rb.get(1) << 24) |
//                         ((uint64_t)ctx.rb.get(2) << 16) |
//                         ((uint64_t)ctx.rb.get(3) << 8) |
//                         ctx.rb.get(4);
        
//         for (int i = 8; i >= 0; i--) {
//             if (((temp >> i) & 0xFFFFFFFF) == syncWord) {
//                 ctx.syncFound = true;
//                 ctx.syncOffset = i;
//                 ctx.status = 1;  // Move to verify
                
//                 if (ctx.enableDebug) {
//                     cout << "[SYNC] Found at offset: " << i << endl;
//                 }
//                 return true;
//             }
//         }
//         ctx.rb.pop();
//     }
//     return false;
// }

// bool DecomProcessor::parseHeaders() {
//     if (!ctx.syncFound) return false;
    
//     // Parse primary header - accesses ctx directly
//     memcpy(&ctx.primaryHeader, ctx.buffer, sizeof(ch10PrimaryHeader));
    
//     // Parse secondary header
//     memcpy(&ctx.secondaryHeader, 
//            ctx.buffer + sizeof(ch10PrimaryHeader),
//            sizeof(ch10SecondaryHeader));
    
//     ctx.status = 2;  // Locked
//     ctx.packetCount++;
    
//     // if (ctx.enableDebug) {
//     //     cout << "[HEADERS] Primary: " << ctx.primaryHeader.channelID 
//     //          << ", Secondary: " << ctx.secondaryHeader.dataLength << endl;
//     // }
    
//     return true;
// }

// bool DecomProcessor::parseChannelData() {
//     // Access ctx members directly - NO parameters!
//     if (!ctx.syncFound || ctx.status < 2) return false;
    
//     // Parse channel specific data...
//     memcpy(&ctx.channelData,
//            ctx.buffer + sizeof(ch10PrimaryHeader) + sizeof(ch10SecondaryHeader),
//            sizeof(channelSpecificData));
    
//     return true;
// }

// void DecomProcessor::reset() {
//     ctx.syncFound = false;
//     ctx.syncOffset = -1;
//     ctx.status = 0;
//     ctx.readIndex = 0;
//     ctx.bitOffset = 0;
// }

// bool DecomProcessor::isLocked() const {
//     return ctx.status == 2;
// }

// const ch10PrimaryHeader& DecomProcessor::getPrimaryHeader() const {
//     return ctx.primaryHeader;
// }

// const ch10SecondaryHeader& DecomProcessor::getSecondaryHeader() const {
//     return ctx.secondaryHeader;
// }

// const channelSpecificData& DecomProcessor::getChannelData() const {
//     return ctx.channelData;
// }

// int DecomProcessor::getPacketCount() const {
//     return ctx.packetCount;
// }

// void DecomProcessor::logDebug(const std::string& message) {
//     if (ctx.enableDebug) {
//         cout << "[DEBUG] " << message << endl;
//     }
// }
