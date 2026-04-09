// #ifndef DECOM_PROCESSOR_H
// #define DECOM_PROCESSOR_H

// #include <cstdint>
// #include <vector>
// #include <memory>
// #include <atomic>
// #include "decomContext.h"
// #include "ch10Header.h"
// #include "ringBuffer.h"

// class DecomProcessor {
// public:
//     // Constructor
//     DecomProcessor(size_t bufferSize = 3200);
    
//     // Public Interface
//     bool processPacket(uint8_t* data, size_t length);
//     bool findSync(uint32_t syncWord);
//     bool parseHeaders();
//     void reset();
//     bool isLocked() const;
    
//     // Getters (read-only access)
//     const ch10PrimaryHeader& getPrimaryHeader() const;
//     const ch10SecondaryHeader& getSecondaryHeader() const;
//     const channelSpecificData& getChannelData() const;
//     int getPacketCount() const;
    
//     // Configuration
//     void enableDebugOutput(bool enable) { ctx.enableDebug = enable; }
//     void enableFileLogging(bool enable) { ctx.enableLogging = enable; }
    
//     // Destructor
//     ~DecomProcessor();
    
// private:
//     // ✅ All member variables here - NO parameter passing needed
//     DecomContext ctx;
//     RingBuffer rb;
//     std::vector<uint8_t> packetBuffer;
    
//     // Private helper functions (access ctx directly)
//     bool validateSync(int offset);
//     bool parseChannelData();
//     void logDebug(const std::string& message);
// };

// #endif // DECOM_PROCESSOR_H
