#ifndef QUEUE_COMMON_H_
#define QUEUE_COMMON_H_

#include <numeric>
#include <chrono>
#include <type_traits>

#include "zmq.hpp"
#include "zmq_addon.hpp"

struct Spike {
    uint8_t layer;
    uint16_t feature;
    uint8_t y;
    uint8_t x;
    uint32_t timestamp;
};

template<typename T>
bool transferPointer = (!std::is_trivially_copyable_v<T> || sizeof(T) > sizeof(void*) * 10);

template<>
bool transferPointer<int> = false;

template<>
bool transferPointer<std::string> = true;

template<>
bool transferPointer<Spike> = true;

#endif