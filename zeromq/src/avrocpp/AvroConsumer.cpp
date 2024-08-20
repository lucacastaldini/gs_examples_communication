#include <fstream>
#include <csignal>
#include <atomic>
#include <condition_variable>

#include "WFSchema.hh"
#include "AvroEntities.hh"
#include "Utils.hh"

#include "consumer.hh"

using namespace WF;

std::queue<std::vector<uint8_t>> serializedQueue;


std::atomic<bool> stop{false};

const int N_mes = 1;
std::vector<HeaderandWaveform> results;

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
 
    auto dser = AvroDeserializer<HeaderandWaveform>(serializedQueue);

    auto t1 = std::chrono::system_clock::now();

    std::cout << "Start Receiving " << std::endl;

    zmq::context_t context(1);
    Consumer<std::vector<uint8_t>> consumer(context, "tcp://" + ip_port);

    int N_cons = 0;

    while (N_cons < N_mes && !stop) {
        std::vector<uint8_t> value;
        if (consumer.consume(value)) {
            serializedQueue.push(std::move(value));  // Push the consumed value into the queue
            ++N_cons;
            if (true) {
                std::cout << "Consumed " << N_cons << " messages." << std::endl;
            }
        }
    }

    auto t2 = std::chrono::system_clock::now();

    N_cons = 0;
    while (N_cons <= serializedQueue.size() && !stop) {
        results.push_back(dser.decode());
    }

    std::cout << "Done" << std::endl;

    auto t3 = std::chrono::system_clock::now();

    std::cout << "First packet" << std::endl;
    print_WF(results[0]);

    std::cout << "Second packet" << std::endl;
    print_WF(results[1]);
    
    std::chrono::duration<double> comm_seconds = t2 - t1;
    std::chrono::duration<double> des_seconds = t3 - t2;

    std::cout << "Consumed number of messages: " << N_mes << std::endl ;
    std::cout << "Communication time : " << comm_seconds.count() << " seconds";
    std::cout << "; " << N_mes/comm_seconds.count() << " packet/s\n";
    std::cout << "Deserialization time : " << des_seconds.count() << " seconds";
    std::cout << "; " << N_mes/des_seconds.count() << " packet/s\n";

    return 0;
}
