#include <fstream>
#include <csignal>
#include <atomic>
#include <condition_variable>
#include <queue>

#include "WFGenerator.hh"
#include "ProtocolBufferEntities.hh"


#include "consumer.hh"

#define N_MES_DEF 1

std::atomic<bool> stop{false};

std::vector<HeaderandWaveform> results;

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

    ThreadSafeQueue<std::string> serializedQueue;
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
 
    MessageReader<HeaderandWaveform> reader(serializedQueue);

    std::cout << "Start Receiving " << std::endl;

    zmq::context_t context(1);
    Consumer<std::string>* consumer = new Consumer<std::string>(context, "tcp://" + ip_port);
    
    int N_cons = 0;
    bool first = true;
    std::chrono::_V2::system_clock::time_point t1;
    while (!stop) {
        std::string value;
        if (consumer->consume(value)) {
            if (first){
                first = false;
                t1 = std::chrono::system_clock::now();
            }
            serializedQueue.push(value);  // Push the consumed value into the queue
            ++N_cons;
            if ( N_cons % 10000  )
                std::cout << "Consumed " << N_cons << " messages." << std::endl;
            
            if( N_cons >= N_mes )
                break;
        }
    }

    auto t2 = std::chrono::system_clock::now();

    N_cons = 0;
    while (!stop) {
        HeaderandWaveform message;
        if(reader.readMessage(message)){
            results.push_back(message);
        }
        if(serializedQueue.size() % 1000 )
            std::cout << "Lenght of queue: " << serializedQueue.size() << std::endl;
        
        // std::cout <<  "Received packet with run id: "<< results.back().runID << ", decimation: " << results.back().decimation << " and pc: " << results.back().counter << std::endl;
        if (serializedQueue.size() <= 0) 
            break; 
    }

    std::cout << "Done" << std::endl;

    auto t3 = std::chrono::system_clock::now();
    
    std::chrono::duration<double> comm_seconds = t2 - t1;
    std::chrono::duration<double> des_seconds = t3 - t2;

    std::cout << "Consumed number of messages: " << N_mes << std::endl ;
    std::cout << "Communication time : " << comm_seconds.count() << " seconds";
    std::cout << "; " << N_mes/comm_seconds.count() << " packet/s\n";
    std::cout << "Deserialization time : " << des_seconds.count() << " seconds";
    std::cout << "; " << N_mes/des_seconds.count() << " packet/s\n";
    std::cout << "Total Receiving time : " << des_seconds.count() + comm_seconds.count() << " seconds";
    std::cout << "; " << N_mes/(des_seconds.count() + comm_seconds.count()) << " packet/s\n";

    delete consumer;
    context.close();

    return 0;
}
