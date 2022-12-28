#ifndef ZMQ_PULLER_H_
#define ZMQ_PULLER_H_

#include "common.h"
#include "encoder.h"

#include <functional>
#include <optional>

// Not thread safe!
template<typename T>
class ZMQPuller
{
public:
    ZMQPuller(zmq::context_t& context, const std::string& endpoint, bool bind=false):
        endpoint(endpoint),
        socket(context, zmq::socket_type::pull)
    {
        if (bind) {
            socket.bind(endpoint);
        }
        else {
            socket.connect(endpoint);
        }
        socket.set(zmq::sockopt::linger, 0);

        if (transferPointer<T>) {
            encoder = std::make_unique<AddressEncoder<T>>();
        }
        else {
            encoder = std::make_unique<ValueEncoder<T>>();
        }
    }

    ~ZMQPuller() 
    {
        // If data is transfered by address, we need to make sure that each address is received and released to prevent memory leak.
        // Currently I just receive all messages then close socket which is not enough to make sure of that.
        // An approach to ensure it decently is to change the socket type from push/pull to router/dealer and always make another side aware of leaving of current side, so another side can stop sending address to this side or receiving any address from this side. This is only necessary when you use `AddressEncoder`.
        zmq::message_t message;
        while (socket.recv(message, zmq::recv_flags::dontwait)) {
            encoder->clear(std::move(message));
        }
        socket.close();
    }

    bool receive(T& data)
    {
        zmq::message_t message;
        if (socket.recv(message, zmq::recv_flags::dontwait)) {
            data = encoder->decode(std::move(message));
            return true;
        }
        else {
            return false;
        }
    }

    bool blockingReceive(T& data, std::chrono::milliseconds timeout)
    {
        zmq::pollitem_t items[] = {
            {socket, 0, ZMQ_POLLIN, 0},
        };
        zmq::poll(&items[0], sizeof(items) / sizeof(zmq::pollitem_t), timeout);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            const auto opt = socket.recv(message, zmq::recv_flags::dontwait);
            assert(opt != std::nullopt);
            data = encoder->decode(std::move(message));
            return true;
        }
        else {
            return false;
        }
    }

    zmq::socket_t* getSocket() { return &socket; }

private:
    zmq::socket_t socket;
    const std::string endpoint;
    std::unique_ptr<Encoder<T>> encoder;
};

#endif