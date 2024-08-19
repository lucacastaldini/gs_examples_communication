#include <random>
#include <fstream>
#include <csignal>
#include <atomic>
#include <condition_variable>

#include "producer.hh"

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
    Producer<int> producer(context, "tcp://" + ip_port);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    int N_mes = 100000000;
    for (int i = 0; i < N_mes && !stop; ++i) {
        int randomValue = distr(gen);
        producer.produce(randomValue);
    }

    return 0;
}
