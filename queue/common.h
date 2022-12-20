#ifndef QUEUE_COMMON_H_
#define QUEUE_COMMON_H_

#include <numeric>
#include <chrono>

#include "zmq.hpp"
#include "zmq_addon.hpp"

constexpr int MAX_QUEUE_SIZE = std::numeric_limits<int>::max();

class SocketQueue
{
public:
    virtual void send(zmq::message_t&&) = 0;        // Block on sending if queue is full
    virtual bool recv(zmq::message_t&) = 0;         // No block
    virtual bool blockingRecv(zmq::message_t&, std::chrono::milliseconds timeout) = 0;  // Block on receive
    virtual zmq::socket_t* getRecvSocket() = 0;

    static zmq::context_t& getContext() {
        static zmq::context_t context;
        return context;
    }
};

#endif