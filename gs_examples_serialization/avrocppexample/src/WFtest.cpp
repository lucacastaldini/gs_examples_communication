#include <iostream>

#include "WFSchema.hh"
#include "AvroEntities.hh"
#include "WFGenerator.hh"
#include "Utils.hh"

using namespace WF;

std::queue<std::vector<uint8_t>> serializedQueue;

int main() {

    auto generator = WFGenerator(1);
    
    auto ser = AvroSerializer<HeaderandWaveform>(serializedQueue);
    auto dser = AvroDeserializer<HeaderandWaveform>(serializedQueue);

    HeaderandWaveform genval = generator.get();
    print_data(genval);
    ser.encode(&genval);
    std::cout << "MAIN:Lenght of queue: " << serializedQueue.size() << std::endl;

    HeaderandWaveform retval= dser.decode();

    if ( areEqual(retval, genval) ){
        std::cout << "Serialization successful" << std::endl;
    }
    else{
        std::cout << "Serialization failed" << std::endl;
    }

}