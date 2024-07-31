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

#endif // CONSUMER_HH
