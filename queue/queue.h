#ifndef QUEUE_H_
#define QUEUE_H_

#include "common.h"
#include "gatherQueue.h"
#include "encoder.h"

#include <memory>
#include <type_traits>

template<typename T, bool transferAddress = (!std::is_trivially_copyable_v<T> || sizeof(T) > sizeof(void*) * 10) >
class Zmqueue
{
public:
    Zmqueue(int maxSize = MAX_QUEUE_SIZE)
    {
        if constexpr (transferAddress) {
            encoder = std::make_unique<AddressEncoder<T>>();
        }
        else {
            encoder = std::make_unique<ValueEncoder<T>>();
        }

        socketQueue = std::make_unique<GatherQueue>(maxSize, [this](zmq::message_t&& msg){ 
            encoder->clear(std::move(msg));
        });
    }

    void send(T&& data)
    {
        zmq::message_t message = encoder->encode(std::move(data));
        socketQueue->send(std::move(message));
    }

    void send(const T& data)
    {
        zmq::message_t message = encoder->encode(data);
        socketQueue->send(std::move(message));
    }

    bool receive(T& data)
    {
        zmq::message_t message;
        if (socketQueue->recv(message)) {
            data = encoder->decode(std::move(message));
            return true;
        }
        else {
            return false;
        }
    }

    bool blockingReceive(T& data, std::chrono::milliseconds timeout)
    {
        zmq::message_t message;
        if (socketQueue->blockingRecv(message, timeout)) {
            data = encoder->decode(std::move(message));
            return true;
        }
        else {
            return false;
        }
    }

private:
    std::unique_ptr<Encoder<T>> encoder;
    std::unique_ptr<GatherQueue> socketQueue;
};

#endif