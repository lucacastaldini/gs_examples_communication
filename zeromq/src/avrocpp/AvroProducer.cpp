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

#define N_MES_DEF 1

using namespace WF;

std::atomic<bool> stop{false};

void signalHandler(int signal) {
    if (signal == SIGINT) {
        stop = true;
        std::cout << "\nSIGINT received. Setting stop flag." << std::endl;
    }
}

int main(int argc, char* argv[]) {

    int N_mes = N_MES_DEF;
    if (argc > 1) {
        // Check if the second argument is a valid integer
        try {
            N_mes = std::stoi(argv[1]); // Convert the argument to an integer
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << argv[1] << ". Using default value of " << N_mes << "." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Argument out of range: " << argv[1] << ". Using default value of " << N_mes << "." << std::endl;
        }
    } else {
        std::cout << "No argument provided. Using default value of " << N_mes << "." << std::endl;
    }

    std::queue<std::vector<uint8_t>> serializedQueue;
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
        ser.encode(&genval);
        
        // std::cout <<  "Sending packet with run id: "<< genval.runID << ", decimation: " << genval.decimation << " and pc: " << genval.counter << std::endl;
       
        if ( i % 10000 == 0 )
            std::cout << "Serialized " << i << "msgs" << std::endl;
        
    }
    std::cout << "MAIN:Lenght of queue: " << serializedQueue.size() << std::endl;

    zmq::context_t context(1);
    Producer<std::vector<uint8_t>>* producer = new Producer<std::vector<uint8_t>>(context, "tcp://" + ip_port);

    auto t2 = std::chrono::system_clock::now();

    std::cout << "Start Sending" << std::endl;
    
    while (!stop)
    {
        producer->produce(serializedQueue.front());
        serializedQueue.pop();
        
        if(serializedQueue.size() % 1000 == 0)
            std::cout << "Lenght of queue: " << serializedQueue.size() << std::endl;
        /* code */
        if (serializedQueue.size()<=0){
            break;
        }
    }
    
    
    std::cout << "Done" << std::endl;

    auto t3 = std::chrono::system_clock::now();
    
    std::chrono::duration<double> serialization_seconds = t2 - t1;
    std::chrono::duration<double> comm_seconds = t3 - t2;
    

    // Display the time difference in seconds
    std::cout << "Produced number of messages: " << N_mes << std::endl ;
    std::cout << "Serialization time : " << serialization_seconds.count() << " seconds";
    std::cout << "; " << N_mes/serialization_seconds.count() << " packet/s\n";
    std::cout << "Communication time : " << comm_seconds.count() << " seconds";
    std::cout << "; " << N_mes/comm_seconds.count() << " packet/s\n";
    delete producer;
    context.close();

    return 0;
 
}
