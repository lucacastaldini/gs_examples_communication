#include <fstream>
#include <csignal>
#include <atomic>
#include <condition_variable>

#include "consumer.hh"

std::atomic<bool> stop{false};

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

    zmq::context_t context(1);
    Consumer<int> consumer(context, "tcp://" + ip_port);

    int N_cons = 0;
    int N_mes = 100000000;

    while (N_cons < N_mes && !stop) {
        int value;
        if (consumer.consume(value)) {
            ++N_cons;
            if (N_cons % 1000000 == 0) {
                std::cout << "Consumed " << N_cons << " messages." << std::endl;
            }
        }
    }

    return 0;
}
