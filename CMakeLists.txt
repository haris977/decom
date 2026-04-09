cmake_minimum_required(VERSION 3.10)
project(DECOM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DECOM_UDP_TEST 
    main_udp.cpp
    src/getShiftedByte.cpp
    src/packetSyncFind.cpp
    src/parseHeader.cpp
    src/searching.cpp
    src/syncFindWithMask.cpp
    src/frameWordFind.cpp
    src/debugHeader.cpp
    src/decomProcessor.cpp
    src/global.cpp
)

target_include_directories(DECOM_UDP_TEST PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
    target_link_libraries(DECOM_UDP_TEST PRIVATE ws2_32)
endif()