#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <google/protobuf/message.h>
 
#include "WFGenerator.hh"
#include "HKGenerator.hh"
#include "TimeSpecGen.hh"
#include "Utils.hh"
#include "ProtocolBufferEntities.hh"

#include "HK.pb.h"  // Include your generated header file for protobuf messages
#include "WF.pb.h"


int main() {
    ThreadSafeQueue<std::string> sharedQueue;
    MessageWriter<HeaderHK> writer(sharedQueue);
    MessageReader<HeaderHK> reader(sharedQueue);


    auto message_gen =  HKGenerator(1);
    // auto message_gen2 =  HKGenerator(1);
    
    // Generate a message and write it to the queue
    int N = 50;
    std::vector<HeaderHK> message(N) ;
    for (int i =0 ; i < N; i++){
        message[i] = message_gen.get();
        writer.writeMessage(message[i]);
    }

    // // Read the message from the queue
    std::vector<HeaderHK> recv_message(N) ;
    for (int i =0 ; i < N; i++){
    if (reader.readMessage(recv_message[i])) {
        std::cout << "Received HeaderHK message:" << std::endl;
        std::cout << "APID: " << recv_message[i].apid() << std::endl;
        std::cout << "Counter: " << recv_message[i].counter() << std::endl;
        // Output other fields as needed
    } else {
        std::cerr << "Failed to read the message." << std::endl;
    }
    // print_HK(recv_message[i]);
    }
    return 0;
}