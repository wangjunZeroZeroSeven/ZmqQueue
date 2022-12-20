#ifndef QUEUE_ENCODER_H_
#define QUEUE_ENCODER_H_

#include "zmq.hpp"

#include <type_traits>

template<typename T>
struct Encoder
{
    virtual zmq::message_t encode(T&&) = 0;

    virtual zmq::message_t encode(const T&) = 0;

    virtual T decode(zmq::message_t&&) = 0;

    virtual void clear(zmq::message_t&&) = 0;
};

template<typename T>
struct ValueEncoder: Encoder<T>
{
    using TD = std::decay_t<T>;

    zmq::message_t encode(T&& data) override
    {
        return zmq::message_t{static_cast<const void*>(&data), sizeof(TD)};
    }

    zmq::message_t encode(const T& data) override
    {
        return zmq::message_t{static_cast<const void*>(&data), sizeof(TD)};
    }

    T decode(zmq::message_t&& msg) override
    {
        assert(msg.size() == sizeof(TD));
        return msg.data<TD>()[0];
    }

    void clear(zmq::message_t&&) override
    {
    }
};

template<typename T>
struct AddressEncoder: Encoder<T>
{
    using TD = std::decay_t<T>;

    zmq::message_t encode(T&& data) override
    {
        TD* ptr = new TD(std::move(data));
        const void* dataPtr = static_cast<const void*>(ptr);
        return zmq::message_t{static_cast<const void*>(&dataPtr), sizeof(void*)};
    }

    zmq::message_t encode(const T& data) override
    {
        TD* ptr = new TD(data);
        const void* dataPtr = static_cast<const void*>(ptr);
        return zmq::message_t{static_cast<const void*>(&dataPtr), sizeof(void*)};
    }

    T decode(zmq::message_t&& msg) override
    {
        assert(msg.size() == sizeof(void*));
        void* data = msg.data<void*>()[0];
        TD* ptr = reinterpret_cast<TD*>(data);

        TD result;
        if constexpr (std::is_move_assignable_v<TD>) {
            result = std::move(*ptr);
        }
        else {
            result = *ptr;
        }

        delete ptr;

        return result;
    }

    void clear(zmq::message_t&& msg) override
    {
        assert(msg.size() == sizeof(void*));
        void* data = msg.data<void*>()[0];
        TD* ptr = reinterpret_cast<TD*>(data);
        delete ptr;
    }
};


#endif