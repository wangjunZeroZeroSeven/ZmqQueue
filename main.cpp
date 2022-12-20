#include <iostream>
#include "queue/queue.h"

#include <thread>
#include <string>

void test_int()
{
    Zmqueue<int> queue;

    std::thread th([&queue](){
        queue.send(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        queue.send(6);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        queue.send(7);
        queue.send(8);
    });

    int data = 0;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "first data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;

    th.join();
}

void test_string()
{
    Zmqueue<std::string> queue;

    std::thread th([&queue](){
        queue.send("aaa");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        queue.send("bbb");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        queue.send("ccc");
        queue.send("ddd");
    });

    std::string data;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "first data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;
    queue.blockingReceive(data, std::chrono::milliseconds(100));
    std::cout << "second data: " << data << std::endl;

    assert(!queue.receive(data));

    th.join();  
}

int main()
{
    test_string();
    test_int();

    return 0;
}