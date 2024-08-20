#ifndef CONSUMER_HH
#define CONSUMER_HH

#include <zmq.hpp>
#include <iostream>

template <typename T>
class Consumer {
public:
    Consumer(zmq::context_t& context, const std::string& address)
        : socket(context, ZMQ_PULL) {
        socket.connect(address);
    }

    // Destructor to clean up the socket
    virtual ~Consumer() {
        try {
            socket.close();
        } catch (const zmq::error_t& e) {
            std::cerr << "Error closing socket: " << e.what() << std::endl;
        }
    }

    bool consume(T& value) {
        zmq::message_t message;
        if (socket.recv(message, zmq::recv_flags::none)) {
            memcpy(&value, message.data(), sizeof(T));
            return true;
        }
        return false;
    }

private:
    zmq::socket_t socket;
};

// Specialization for std::vector<T>
template <typename T>
class Consumer<std::vector<T>> {
public:
    Consumer(zmq::context_t& context, const std::string& address)
        : socket(context, ZMQ_PULL) {
        socket.connect(address);
    }

    // Destructor to clean up the socket
    virtual ~Consumer() {
        try {
            socket.close();
        } catch (const zmq::error_t& e) {
            std::cerr << "Error closing socket: " << e.what() << std::endl;
        }
    }

    bool consume(std::vector<T>& vec) {
        zmq::message_t message;
        if (socket.recv(message, zmq::recv_flags::none)) {
            size_t size ;
            memcpy( &size, message.data(), sizeof(size_t));

            // std::cout << "msg size is : " << size << std::endl;
            vec.resize(size);
            
            memcpy( vec.data(), static_cast<char*>(message.data()) + sizeof(size_t),  size * sizeof(T));
            return true;
        }
        return false;
        // Serialize the vector size and elements into a single buffer
        }

private:
    zmq::socket_t socket;
};

#endif // CONSUMER_HH
