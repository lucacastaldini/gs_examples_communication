#include "WF.pb.h"
#include "HK.pb.h"

void print_HK(HeaderHK h) {
    std::cout << "abstime: " << h.abstime() << std::endl;
    std::cout << "apid: " << h.apid() << std::endl;
    std::cout << "configID: " << h.configid() << std::endl;
    std::cout << "counter: " << h.counter() << std::endl;
    std::cout << "crc: " << h.crc() << std::endl;
    std::cout << "flags: " << h.flags() << std::endl;
    std::cout << "runID: " << h.runid() << std::endl;
    std::cout << "time s: " << h.time().tv_sec() << std::endl;
    std::cout << "time ns: " << h.time().tv_nsec() << std::endl;
    std::cout << "type: " << h.type() << std::endl;
    std::cout << "wformcount: " << h.wformcount() << std::endl;
}

// Function to print HeaderandWaveform
void print_WF(const HeaderandWaveform& h) {
    std::cout << "abstime: " << h.abstime() << std::endl;
    std::cout << "apid: " << h.apid() << std::endl;
    std::cout << "configID: " << h.configid() << std::endl;
    std::cout << "counter: " << h.counter() << std::endl;
    std::cout << "crc: " << h.crc() << std::endl;
    std::cout << "runID: " << h.runid() << std::endl;
    std::cout << "time s: " << h.time().tv_sec() << std::endl;
    std::cout << "time ns: " << h.time().tv_nsec() << std::endl;
    std::cout << "type: " << h.type() << std::endl;
    std::cout << "equalizationLevel: " << h.equalizationlevel() << std::endl;
    std::cout << "decimation: " << h.decimation() << std::endl;
    std::cout << "curOffset: " << h.curoffset() << std::endl;
    std::cout << "trigOff: " << h.trigoff() << std::endl;
    std::cout << "size: " << h.size() << std::endl;
    std::cout << "data: ";
    for (std::size_t i = 0; i < h.data().size(); ++i) {
        std::cout << h.data().Get(i) << " ";
    }
    std::cout << std::endl;
}

// // Function to compare TimeSpec instances
// bool areEqual(const HK::TimeSpec& lhs, const HK::TimeSpec& rhs) {
//     return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
// }

// bool areEqual(const WF::TimeSpec& lhs, const WF::TimeSpec& rhs) {
//     return (lhs.tv_sec == rhs.tv_sec) && (lhs.tv_nsec == rhs.tv_nsec);
// }

// // Function to compare HeaderHK instances
// bool areEqual(const HeaderHK& lhs, const HeaderHK& rhs) {
//     return (lhs.apid == rhs.apid) &&
//            (lhs.counter == rhs.counter) &&
//            (lhs.type == rhs.type) &&
//            areEqual(lhs.time, rhs.time) &&
//            (lhs.abstime == rhs.abstime) &&
//            (lhs.runID == rhs.runID) &&
//            (lhs.configID == rhs.configID) &&
//            (lhs.wformcount == rhs.wformcount) &&
//            (lhs.flags == rhs.flags) &&
//            (lhs.crc == rhs.crc);
// }

// bool areEqual(const HeaderandWaveform& lhs, const HeaderandWaveform& rhs) {
//     return (lhs.apid == rhs.apid) &&
//            (lhs.counter == rhs.counter) &&
//            (lhs.type == rhs.type) &&
//            areEqual(lhs.time, rhs.time) &&
//            (lhs.abstime == rhs.abstime) &&
//            (lhs.runID == rhs.runID) &&
//            (lhs.configID == rhs.configID) &&
//            (lhs.equalizationLevel == rhs.equalizationLevel) &&
//            (lhs.decimation == rhs.decimation) &&
//            (lhs.curOffset == rhs.curOffset) &&
//            (lhs.trigOff == rhs.trigOff) &&
//            (lhs.size == rhs.size) &&
//            (lhs.data == rhs.data) &&  // Compares the data vectors
//            (lhs.crc == rhs.crc);
// }