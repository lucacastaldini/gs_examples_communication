#ifndef PRODUCER_HH
#define PRODUCER_HH

#include <zmq.hpp>
#include <iostream>

template <typename T>
class Producer
{
public:
    Producer(zmq::context_t &context, const std::string &address)
        : socket(context, ZMQ_PUSH)
    {
        socket.bind(address);
    }

    // Destructor to clean up the socket
    virtual ~Producer()
    {
        try
        {
            socket.close();
        }
        catch (const zmq::error_t &e)
        {
            std::cerr << "Error closing socket: " << e.what() << std::endl;
        }
    }

    void produce(const T &value)
    {
        zmq::message_t message(sizeof(T));
        memcpy(message.data(), &value, sizeof(T));

        try
        {
            if (!socket.send(message, zmq::send_flags::none))
            {
                std::cerr << "Error sending message." << std::endl;
            }
        }
        catch (const zmq::error_t &e)
        {
            std::cerr << "Error sending message: " << e.what() << std::endl;
        }
    }

    private:
        zmq::socket_t socket;
    };

    // Specialization for std::vector<T>
    template <typename T>
    class Producer<std::vector<T>>
    {
    public:
        Producer(zmq::context_t &context, const std::string &address)
            : socket(context, ZMQ_PUSH)
        {
            socket.bind(address);
        }

        // Destructor to clean up the socket
        virtual ~Producer()
        {
            try
            {
                socket.close();
            }
            catch (const zmq::error_t &e)
            {
                std::cerr << "Error closing socket: " << e.what() << std::endl;
            }
        }

        void produce(const std::vector<T> &vec)
        {
            // Serialize the vector size and elements into a single buffer
            size_t size = vec.size();
            // std::cout << "msg size is : " << size << std::endl;
            zmq::message_t message(sizeof(size_t) + size * sizeof(T));

            // Copy the size and data into the message buffer
            memcpy(message.data(), &size, sizeof(size_t));
            memcpy(static_cast<char *>(message.data()) + sizeof(size_t), vec.data(), size * sizeof(T));

            // Send the message
            socket.send(message, zmq::send_flags::none);
        }

    private:
        zmq::socket_t socket;
    };

    // Specialization for std::string
    template <>
    class Producer<std::string>
    {
    public:
        Producer(zmq::context_t &context, const std::string &address)
            : socket(context, ZMQ_PUSH)
        {
            socket.bind(address);
        }

        virtual ~Producer()
        {
            try
            {
                socket.close();
            }
            catch (const zmq::error_t &e)
            {
                std::cerr << "Error closing socket: " << e.what() << std::endl;
            }
        }

        void produce(const std::string &str)
        {
            // Serialize the string length and data into a single buffer
            size_t size = str.size();
            zmq::message_t message(sizeof(size_t) + size);

            // Copy the size and data into the message buffer
            memcpy(message.data(), &size, sizeof(size_t));
            memcpy(static_cast<char *>(message.data()) + sizeof(size_t), str.data(), size);

            // Send the message
            socket.send(message, zmq::send_flags::none);
        }

    private:
        zmq::socket_t socket;
    };

#endif // PRODUCER_HH