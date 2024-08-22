#include <iostream>

#include "HKSchema.hh"
#include "AvroEntities.hh"
#include "HKGenerator.hh"
#include "Utils.hh"

using namespace HK;

std::queue<std::vector<uint8_t>> serializedQueue;

int main() {

    HKGenerator generator = HKGenerator(1);
    
    auto ser = AvroSerializer<HeaderHK>(serializedQueue);
    auto dser = AvroDeserializer<HeaderHK>(serializedQueue);

    HeaderHK genval = generator.get();
    print_data(genval);
    ser.encode(&genval);
    std::cout << "MAIN:Lenght of queue: " << serializedQueue.size() << std::endl;

    HeaderHK retval= dser.decode();

    if ( areEqual(retval, genval) ){
        std::cout << "Serialization successful" << std::endl;
    }
    else{
        std::cout << "Serialization failed" << std::endl;
    }

}