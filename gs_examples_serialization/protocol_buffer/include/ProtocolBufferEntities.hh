#ifndef PB_ENTITIES_HH
#define PB_ENTITIES_HH

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <google/protobuf/message.h>
#include "HK.pb.h"  // Include your generated header file for protobuf messages

// Thread-safe queue implementation
template <typename T>
class ThreadSafeQueue {
public:
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        cond_var_.notify_one();  // Notify any waiting threads
    }

    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this]() { return !queue_.empty(); });
        item = queue_.front();
        queue_.pop();
        return true;
    }

    size_t size(){
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

// Writer class
template <class T> class MessageWriter {
public:
    MessageWriter(ThreadSafeQueue<std::string>& queue) : queue_(queue) {}

    void writeMessage(const T& message) {
        // Serialize the message to a string
        std::string serialized_message ;
        if (!message.SerializeToString(&serialized_message)) {
            std::cerr << "Failed to serialize the message." << std::endl;
            return;
        }
        // std::cout << "Serialized size:" << serialized_message.size() << std::endl;

        // Push the serialized message into the shared queue
        queue_.push(serialized_message);

        serialized_message.clear();
    }

private:
    ThreadSafeQueue<std::string>& queue_;
};

// Reader class
template <class T> class MessageReader {
public:
    MessageReader(ThreadSafeQueue<std::string>& queue) : queue_(queue) {}

    bool readMessage(T& message) {
        std::string serialized_message;
        if (!queue_.pop(serialized_message)) {
            std::cerr << "Failed to pop message from queue." << std::endl;
            return false;
        }

        // Deserialize the message from the string
        if (!message.ParseFromString(serialized_message)) {
            std::cerr << "Failed to deserialize the message." << std::endl;
            return false;
        }

        return true;
    }

private:
    ThreadSafeQueue<std::string>& queue_;
};

#endif