#include <iostream>
#include "src/zmqPuller.h"
#include "src/zmqPusher.h"
#include "src/uuidGen.h"

#include <moodycamel/blockingconcurrentqueue.h>

#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <type_traits>

template<typename T>
T generateSingleData() {
    if constexpr (std::is_same_v<T, int>) {
        return 1;
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        return std::string("Hello World!");
    }
    else if constexpr (std::is_same_v<T, Spike>) {
        return Spike{1, 2, 5, 6, 567};
    }
    else {
        throw "Unsupported type!";
    }
}

template<typename T>
void test_zmq_transfer(const size_t data_count, const size_t send_thread_count = 1)
{
    const std::string endpoint = std::string("inproc://zmq-") + getUuid();
    zmq::context_t context;

    std::vector<std::unique_ptr<std::thread>> ths;
    T data = generateSingleData<T>();
    for (int i = 0; i < send_thread_count; i++) {
        const size_t send_count = (i < send_thread_count - 1) ? data_count / send_thread_count : data_count - (data_count / send_thread_count) * (send_thread_count - 1);
        ths.push_back(std::make_unique<std::thread>([data, &endpoint, &context, send_count](){
            ZMQPusher<T> pusher(context, endpoint, false);
            for (int i = 0; i < send_count; i++) {
                pusher.send(data);
            }
        }));
    }

    const auto begin = std::chrono::steady_clock::now();

    // receive
    ZMQPuller<T> puller(context, endpoint, true);
    size_t recv_count = 0;
    T recvData;
    while (recv_count < data_count) {
        if (puller.receive(recvData)) {
            recv_count++;
        } 
    }

    const auto durationMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "All received for zmq, cost milliseconds: " << durationMilliSec << std::endl;

    for (int i = 0; i < send_thread_count; i++) {
        ths[i]->join();
    }
}

template<typename T>
void test_ccqueue_transfer(const size_t data_count, const size_t send_thread_count = 1)
{
    moodycamel::BlockingConcurrentQueue<T> concurrentQueue;

    std::vector<std::unique_ptr<std::thread>> ths;
    T data = generateSingleData<T>();
    for (int i = 0; i < send_thread_count; i++) {
        const size_t send_count = (i < send_thread_count - 1) ? data_count / send_thread_count : data_count - (data_count / send_thread_count) * (send_thread_count - 1);
        ths.push_back(std::make_unique<std::thread>([data, &concurrentQueue, send_count](){
            for (int i = 0; i < send_count; i++) {
                concurrentQueue.enqueue(data);
            }
        }));
    }

    const auto begin = std::chrono::steady_clock::now();

    // receive
    size_t recv_count = 0;
    T recvData;
    while (recv_count < data_count) {
        if (concurrentQueue.try_dequeue(recvData)) {
            recv_count++;
        } 
    }

    const auto durationMilliSec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
    std::cout << "All received for cc queue, cost milliseconds: " << durationMilliSec << std::endl;

    for (int i = 0; i < send_thread_count; i++) {
        ths[i]->join();
    }
}

// void test_string()
// {
//     Zmqueue<std::string> queue;

//     std::thread th([&queue](){
//         queue.send("aaa");
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         queue.send("bbb");
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         queue.send("ccc");
//         queue.send("ddd");
//     });

//     std::string data;
//     queue.blockingReceive(data, std::chrono::milliseconds(100));
//     std::cout << "first data: " << data << std::endl;
//     queue.blockingReceive(data, std::chrono::milliseconds(100));
//     std::cout << "second data: " << data << std::endl;
//     queue.blockingReceive(data, std::chrono::milliseconds(100));
//     std::cout << "second data: " << data << std::endl;
//     queue.blockingReceive(data, std::chrono::milliseconds(100));
//     std::cout << "second data: " << data << std::endl;

//     assert(!queue.receive(data));

//     th.join();  
// }

int main()
{
    std::cout << "Good" << std::endl;
    // test_zmq_transfer<int>(100000000, 3);
    // test_zmq_transfer<std::string>(100000000, 3);
    test_zmq_transfer<Spike>(100000000, 3);


    // test_ccqueue_transfer<int>(100000000, 3);
    // test_ccqueue_transfer<std::string>(100000000, 3);
    test_ccqueue_transfer<Spike>(100000000, 3);

    // test_string();
    // test_int();

    return 0;
}