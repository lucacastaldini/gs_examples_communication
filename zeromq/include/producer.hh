#ifndef PRODUCER_HH
#define PRODUCER_HH

#include <zmq.hpp>
#include <iostream>

template <typename T>
class Producer {
public:
    Producer(zmq::context_t& context, const std::string& address)
        : socket(context, ZMQ_PUSH) {
        socket.bind(address);
    }

    void produce(const T& value) {
        zmq::message_t message(sizeof(T));
        memcpy(message.data(), &value, sizeof(T));
        socket.send(message, zmq::send_flags::none);
    }

private:
    zmq::socket_t socket;
};

#endif // PRODUCER_HH
