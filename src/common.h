#ifndef QUEUE_COMMON_H_
#define QUEUE_COMMON_H_

#include <numeric>
#include <chrono>
#include <type_traits>

#include "zmq.hpp"
#include "zmq_addon.hpp"

#include "event.h"

#include <vector>
#include <memory>

template<typename T>
bool transferPointer = (!std::is_trivially_copyable_v<T> || sizeof(T) > sizeof(void*) * 10);

template<>
bool transferPointer<int> = false;

template<>
bool transferPointer<std::string> = true;

template<>
bool transferPointer<Spike> = true;

template<>
bool transferPointer<OutputEvent> = true;

template<>
bool transferPointer<std::shared_ptr<std::vector<OutputEvent>>> = true;

#endif