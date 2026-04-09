// #ifndef DECOM_CONTEXT_H
// #define DECOM_CONTEXT_H

// #include <cstdint>
// #include <atomic>
// #include "ch10Header.h"
// #include "ringBuffer.h"

// // ✅ SINGLE CONTEXT STRUCT - Replace 10+ parameters with ONE
// struct DecomContext {
//     // Raw Data / Buffer
//     RingBuffer& rb;                  // Ring buffer reference
//     uint8_t* buffer;                 // Current packet buffer
//     size_t bufferLength;            // Current packet length
//     size_t readIndex;               // Current read position
//     size_t bitOffset;               // Bit-level offset for parsing
    
//     // Headers (Parsed)
//     ch10PrimaryHeader primaryHeader;
//     ch10SecondaryHeader secondaryHeader;
    
//     // Channel Specific Data
//     channelSpecificData channelData;
    
//     // Runtime State
//     int status;                      // 0 = searching, 1 = verify, 2 = locked
//     bool syncFound;                  // Flag: sync pattern found
//     int syncOffset;                  // Bit offset of sync pattern
//     int packetCount;                 // Total packets processed
    
//     // Channel Tracking
//     bool channel[1000];              // Channel presence flags
//     uint8_t channelType[1];          // Channel type
    
//     // Configuration / Flags
//     bool enableDebug;                // Debug output flag
//     bool enableLogging;              // File logging flag
    
//     // Threading (if needed)
//     std::atomic<bool> ready;         // Data ready flag
//     std::atomic<bool> stop;          // Stop processing flag
    
//     // Constructor
//     DecomContext(RingBuffer& ring_buffer) 
//         : rb(ring_buffer), 
//           buffer(nullptr), 
//           bufferLength(0),
//           readIndex(0),
//           bitOffset(0),
//           status(0),
//           syncFound(false),
//           syncOffset(-1),
//           packetCount(0),
//           enableDebug(false),
//           enableLogging(false),
//           ready(false),
//           stop(false)
//     {
//         memset(channel, 0, sizeof(channel));
//         memset(channelType, 0xFF, sizeof(channelType));
//         memset(&primaryHeader, 0, sizeof(primaryHeader));
//         memset(&secondaryHeader, 0, sizeof(secondaryHeader));
//         memset(&channelData, 0, sizeof(channelData));
//     }
// };

// #endif // DECOM_CONTEXT_H
