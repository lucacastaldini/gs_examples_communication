//
//  packet.h
//  Packet
//
//  Created by Alessio Aboudan on 20/05/21.
//

#ifndef __PACKET_H__
#define __PACKET_H__

#include <cstdint>
#include <time.h>
#include <array>

#define TC_RX_OK 			0x00
#define TC_RX_ERR_WRONGSTT 	0xFF

#define TC_EX_OK 			0x00
#define TC_EX_ERR_RUPDFAIL 	0xFE
#define TC_EX_ERR_INITFAIL  0xFD

#define WF_SIZE 16384
// Define the HeaderHK struct
class Header {
public:
    // Header data
    union {
        uint8_t _p8[28];   // 7 * 4 bytes for 7 integers
        uint16_t _p16[14]; // 7 * 2 2-byte integers
        uint32_t _p32[7];  // 7 integers
        struct __attribute__((packed)) {
            int apid;
            int counter;
            int type;
            struct timespec ts;
            int abstime;
            int runID;
            int configID;
        };
    };

    static void print(const Header& header);
};

// Define the Data_Hk struct
class Data_Hk {
public:

    // Header data
    union {
        uint8_t _p8[12];   // 3 * 4 bytes for 3 integers
        uint16_t _p16[6];  // 3 * 2 2-byte integers
        uint32_t _p32[3];  // 3 integers
        struct __attribute__((packed)) {
            uint32_t wformcount;
            uint32_t flags;
            uint32_t crc;
        };
    };

    static void print(const Data_Hk& data);
};

// Define the Data_Wf struct
class Data_Wf {
public:

    // Header data
    union {
        uint8_t _p8[(WF_SIZE+6)*4];  
        uint16_t _p16[(WF_SIZE+6)*2];  
        uint32_t _p32[WF_SIZE+6];  
        struct __attribute__((packed)) {
            int32_t equalizationLevel;
            int32_t decimation;
            int32_t curOffset;
            int32_t trigOff;
            int32_t size;
            std::array<int32_t,16384> data;
            int32_t crc;
        };
    };

    static void print(const Data_Wf& data);
};

class HeaderHK {
public:
    struct __attribute__((packed)) {
        Header h;
        Data_Hk d;
    };

    static void print(const HeaderHK& packet);
};

class HeaderWF {
public:
    struct __attribute__((packed)) {
        Header h;
        Data_Wf d;
    };


    static void print(const HeaderWF& packet);
};


#endif // __PACKET_H__
