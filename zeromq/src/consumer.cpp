#include <zmq.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <csignal>
#include <condition_variable>

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
    zmq::socket_t socket(context, ZMQ_PULL);
    socket.connect("tcp://" + ip_port);

    int N_cons = 0;
    int N_mes = 100000000;

    while (N_cons < N_mes && !stop) {
        zmq::message_t message;
        socket.recv(message, zmq::recv_flags::none);
        int value;
        memcpy(&value, message.data(), sizeof(int));
        ++N_cons;
        if (N_cons % 1000000 == 0) {
            std::cout << "Consumed " << N_cons << " messages." << std::endl;
        }
    }

    return 0;
}
