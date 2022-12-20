#ifndef QUEUE_GATHER_QUEUE_H_
#define QUEUE_GATHER_QUEUE_H_

#include "common.h"
#include "uuidGen.h"

#include <functional>

class GatherQueue: public SocketQueue
{
public:
    using MessageClearHandler = std::function<void(zmq::message_t&&)>;

    GatherQueue(int maxSize, MessageClearHandler messageClearHandler = nullptr):
        gatherSocket(getContext(), zmq::socket_type::gather),
        scatterSocket(getContext(), zmq::socket_type::scatter),
        messageClearHandler(messageClearHandler)
    {
        gatherSocket.bind(endpoint);
        gatherSocket.set(zmq::sockopt::rcvhwm, maxSize - maxSize / 2);
        gatherSocket.set(zmq::sockopt::linger, 0);

        scatterSocket.connect(endpoint);
        scatterSocket.set(zmq::sockopt::sndhwm, maxSize / 2);
        scatterSocket.set(zmq::sockopt::linger, 0);
    }

    ~GatherQueue() 
    {
        scatterSocket.close();

        if (messageClearHandler) {
            zmq::message_t message;
            while (gatherSocket.recv(message, zmq::recv_flags::dontwait)) {
                messageClearHandler(std::move(message));
            }
        }
        gatherSocket.close();
    }

    void send(zmq::message_t&& message) override
    {
        scatterSocket.send(std::move(message), zmq::send_flags::none);
    }

    bool recv(zmq::message_t& message) override
    {
        return gatherSocket.recv(message, zmq::recv_flags::dontwait) != std::nullopt;
    }

    bool blockingRecv(zmq::message_t& message, std::chrono::milliseconds timeout) override
    {
        zmq::pollitem_t items[] = {
            {gatherSocket, 0, ZMQ_POLLIN, 0},
        };
        zmq::poll(&items[0], sizeof(items) / sizeof(zmq::pollitem_t), timeout);

        if (items[0].revents & ZMQ_POLLIN) {
            return gatherSocket.recv(message, zmq::recv_flags::dontwait) != std::nullopt;
        }
        else {
            return false;
        }
    }

    zmq::socket_t* getRecvSocket() override { return &gatherSocket; }

private:
    zmq::socket_t gatherSocket;
    zmq::socket_t scatterSocket;
    const std::string endpoint = std::string("inproc://gatherqueue-") + getUuid();
    MessageClearHandler messageClearHandler; 
};

#endif