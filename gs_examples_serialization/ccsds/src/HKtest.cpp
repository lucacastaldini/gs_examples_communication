#include "tchandler.h"
#include "generators.hh"
#include <iostream>
#include <queue>

constexpr size_t buffSz = sizeof(HeaderHK);

int main() {

    std::queue<std::vector<uint8_t>> serializedQueue;

    TcHandler<HeaderHK> handler(serializedQueue);

    HKGenerator generator(1);

    uint8_t buffer[buffSz];  // Use stack allocation for buffer

    // Bufferize the generated structures
    handler.bufferize(generator.get());

    // Variables to hold debufferized data
    HeaderHK debufferizedPack;

    // Debufferize the content back into structures
    handler.debufferize(debufferizedPack);

    // Verify the content of the debufferized data
    std::cout << "Debufferized Header APID: " << debufferizedPack.h.apid << std::endl;
    std::cout << "Debufferized Data Wformcount: " << debufferizedPack.d.wformcount << std::endl;

    HeaderHK::print(debufferizedPack);

    return 0;
}
