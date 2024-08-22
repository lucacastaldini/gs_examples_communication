//
//  packet.cpp
//  Packet
//
//  Created by Alessio Aboudan on 20/05/21.
//

#include "packet.h"

#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>

void Header::print(const Header& header) {
    std::cout << "HeaderHK:" << std::endl;
    std::cout << "  APID: " << header.apid << std::endl;
    std::cout << "  Counter: " << header.counter << std::endl;
    std::cout << "  Type: " << header.type << std::endl;
    std::cout << "  Timestamp: " << header.ts.tv_sec << "s " << header.ts.tv_nsec << "ns" << std::endl;
    std::cout << "  Absolute Time: " << header.abstime << std::endl;
    std::cout << "  Run ID: " << header.runID << std::endl;
    std::cout << "  Config ID: " << header.configID << std::endl;
}

void Data_Hk::print(const Data_Hk& data) {
    std::cout << "Data_Hk:" << std::endl;
    std::cout << "  Waveform Count: " << data.wformcount << std::endl;
    std::cout << "  Flags: 0x" << std::hex << data.flags << std::dec << std::endl;
    std::cout << "  CRC: 0x" << std::hex << data.crc << std::dec << std::endl;
}

void Data_Wf::print(const Data_Wf& data) {
    std::cout << "Data_Wf:" << std::endl;
    std::cout << "  Equalization Level: " << data.equalizationLevel << std::endl;
    std::cout << "  Decimation: " << data.decimation << std::endl;
    std::cout << "  Current Offset: " << data.curOffset << std::endl;
    std::cout << "  Trigger Offset: " << data.trigOff << std::endl;
    std::cout << "  Size: " << data.size << std::endl;
    
    std::cout << "  Data: [";
    for (int i = 0; i < 10; ++i) {  // Print only the first 10 elements for brevity
        std::cout << data.data[i];
        if (i < 9) {
            std::cout << ", ";
        }
    }
    std::cout << ", ...]" << std::endl; // Indicating there are more elements not shown
    
    std::cout << "  CRC: 0x" << std::hex << data.crc << std::dec << std::endl;
}

void HeaderHK::print(const HeaderHK& p) {
    Header::print(p.h);
    Data_Hk::print(p.d);
}

void HeaderWF::print(const HeaderWF& p) {
    Header::print(p.h);
    Data_Wf::print(p.d);
}
