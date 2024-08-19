#include <random>
#include <fstream>
#include <csignal>
#include <atomic>
#include <condition_variable>

#include "WFSchema.hh"
#include "AvroEntities.hh"
#include "WFGenerator.hh"
#include "Utils.hh"

#include "producer.hh"

using namespace WF;

std::queue<std::vector<uint8_t>> serializedQueue;

std::atomic<bool> stop{false};

const int N_mes = 200;

void signalHandler(int signal) {
    if (signal == SIGINT) {
        stop = true;
        std::cout << "\nSIGINT received. Setting stop flag." << std::endl;
    }
}

int main() {
    std::signal(SIGINT, signalHandler);

    std::ifstream config("config.txt");
    std::string ip_port;
    if (config.is_open()) {
        std::getline(config, ip_port);
        config.close();
    } else {
        std::cerr << "Unable to open config file!" << std::endl;
        return 1;
    }

    auto generator = WFGenerator(1);    
    auto ser = AvroSerializer<HeaderandWaveform>(serializedQueue);

    auto t1 = std::chrono::system_clock::now();

    std::cout << "Start Serializing " << std::endl;

    for (int i = 0; i < N_mes && !stop; ++i) {
        HeaderandWaveform genval = generator.get();
        // print_data(genval);
        ser.encode(&genval);
        if (i%100 == 0){
            std::cout << "serialized " << i << "msgs" << std::endl;
        }
    }
    std::cout << "MAIN:Lenght of queue: " << serializedQueue.size() << std::endl;

    zmq::context_t context(1);
    Producer<std::vector<uint8_t>> producer(context, "tcp://" + ip_port);
    
    auto t2 = std::chrono::system_clock::now();

    std::cout << "Start Sending" << std::endl;
    while ( serializedQueue.size() > 0 && !stop) {
        producer.produce(serializedQueue.front());
        serializedQueue.pop();
        std::cout << "Lenght of queue: " << serializedQueue.size() << std::endl;
    }
    std::cout << "Done" << std::endl;

    auto t3 = std::chrono::system_clock::now();
    
    // std::chrono::duration<double> serialization_seconds = t2 - t1;
    // std::chrono::duration<double> comm_seconds = t3 - t2;
    

    // // Display the time difference in seconds
    // std::cout << "Produced number of messages: " << N_mes << std::endl ;
    // std::cout << "Serialization time : " << serialization_seconds.count() << " seconds";
    // std::cout << "; " << N_mes/serialization_seconds.count() << " packet/s\n";
    // std::cout << "Communication time : " << comm_seconds.count() << " seconds";
    // std::cout << "; " << N_mes/comm_seconds.count() << " packet/s\n";
    


    return 0;
}
