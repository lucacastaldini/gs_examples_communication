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
    MessageWriter<HeaderandWaveform> writer(sharedQueue);
    MessageReader<HeaderandWaveform> reader(sharedQueue);


    auto message_gen =  WFGenerator(1);
    // auto message_gen2 =  HKGenerator(1);
    
    // Generate a message and write it to the queue
    // HeaderHK message2 = message_gen2.get();
    // print_WF(message);
    // print_HK(message2);
    // Generate a message and write it to the queue
    int N = 50;
    std::vector<HeaderandWaveform> message(N);
    for (int i =0 ; i < N; i++){
        message[i] = message_gen.get();
        writer.writeMessage(message[i]);
    }

    // // Read the message from the queue
    std::vector<HeaderandWaveform> recv_message(N) ;
    for (int i =0 ; i < N; i++){
    if (reader.readMessage(recv_message[i])) {
        std::cout << "Received HeaderandWaveform message:" << std::endl;
        std::cout << "APID: " << recv_message[i].apid() << std::endl;
        std::cout << "Counter: " << recv_message[i].counter() << std::endl;
        std::cout << "Lenght of data: " << recv_message[i].data_size() << std::endl;
        // Output other fields as needed
    } else {
        std::cerr << "Failed to read the message." << std::endl;
    }
    }
    // print_WF(recv_message);
    return 0;
}