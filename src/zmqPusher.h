#ifndef ZMQ_PUSHER_H_
#define ZMQ_PUSHER_H_

#include "common.h"
#include "encoder.h"

#include <functional>

// Not thread safe!
template<typename T>
class ZMQPusher
{
public:
    ZMQPusher(zmq::context_t& context, const std::string& endpoint, bool bind=false):
        endpoint(endpoint),
        socket(context, zmq::socket_type::push)
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

    ~ZMQPusher() 
    {
        socket.close();
    }

    void send(T&& data)
    {
        zmq::message_t message = encoder->encode(std::move(data));
        socket.send(std::move(message), zmq::send_flags::none);
    }

    void send(const T& data)
    {
        zmq::message_t message = encoder->encode(data);
        socket.send(std::move(message), zmq::send_flags::none);
    }

    zmq::socket_t* getSocket() { return &socket; }

private:
    zmq::socket_t socket;
    const std::string endpoint;
    std::unique_ptr<Encoder<T>> encoder;
};

#endif