#include "tchandler.h"
#include <queue>
#include "generators.hh"
#include <iostream>

int main() {

    std::queue<std::vector<uint8_t>> serializedQueue;

    TcHandler<HeaderWF> handler(serializedQueue);

    WFGenerator generator(1);

    // Bufferize the generated structures
    handler.bufferize(generator.get());

    // Variables to hold debufferized data
    HeaderWF receivedPacket;

    // Debufferize the content back into structures
    handler.debufferize(receivedPacket);

    // Verify the content of the debufferized data
    std::cout << "Debufferized Header APID: " << receivedPacket.h.apid << std::endl;
    std::cout << "Debufferized Data size: " << receivedPacket.d.size << std::endl;

    HeaderWF::print(receivedPacket);

    return 0;
}
