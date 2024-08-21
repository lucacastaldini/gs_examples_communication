#include <csignal>
#include <atomic>
#include <iostream>
#include <fstream>

#include "utils.hh"


std::atomic<bool> stop{false}; 

void signalHandler(int signal) {
    if (signal == SIGINT) {
        stop.store(true);
        std::cout << "\nSIGINT received. Setting stop flag." << std::endl;
    }
}

const int parseArguments(int argc, char* argv[]) {
    int N_mes = N_MES_DEF;

    if (argc > 1) {
        try {
            N_mes = std::stoi(argv[1]);  // Convert the argument to an integer
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << argv[1] << ". Using default value of " << N_mes << "." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Argument out of range: " << argv[1] << ". Using default value of " << N_mes << "." << std::endl;
        }
    } else {
        std::cout << "No argument provided. Using default value of " << N_mes << "." << std::endl;
    }

    return N_mes;
}

const int getMessageUpdate(const int N_mes){
    return N_mes / N_MES_UPDATE_SCALER ;
}

// Function to read the IP and port from a config file
std::string getIpPortFromConfig(const std::string& filename) {
    std::ifstream config(filename);
    std::string ip_port;

    if (config.is_open()) {
        std::getline(config, ip_port);
        config.close();
    } else {
        std::cerr << "Unable to open config file: " << filename << std::endl;
    }

    return ip_port;
}

bool handleZmqOperation(std::function<bool()> operation)
{
    try
    {
        return operation();
    }
    catch (const zmq::error_t& e)
    {
        if (e.num() == EINTR) // Interrupted system call
        {
            std::cerr << "Operation interrupted by signal." << std::endl;
            return false;
        }
        else
        {
            std::cerr << "ZeroMQ error: " << e.what() << std::endl;
            throw; // Re-throw other errors
        }
    }
}

bool receiveMessage(zmq::socket_t& socket, std::function<void(const zmq::message_t&)> processMessage)
{
    zmq::message_t message;
    return handleZmqOperation([&]() -> bool
    {
        if (socket.recv(message, zmq::recv_flags::none))
        {
            processMessage(message);
            return true;
        }
        return false;
    });
}

bool sendMessage(zmq::socket_t& socket, std::function<zmq::message_t()> prepareMessage)
{
    return handleZmqOperation([&]() -> bool
    {
        auto message = prepareMessage();
        socket.send(message, zmq::send_flags::none);
        return true;
    });
}