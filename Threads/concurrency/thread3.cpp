#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

std::mutex gLock;
static int shared_counter = 0;

void incrementCounter() {
    shared_counter = shared_counter + 1;
}


void incrementCounter() {
    gLock.lock();
        shared_counter = shared_counter + 1;
    gLock.unlock();
}

void incrementCounter() {
    std::lock_guard<std::mutex> guard(gLock);
    shared_counter = shared_counter + 1;
}

int main() {

    std::vector<std::thread> threads;
    for (int i = 0; i < 100000; ++i) {
        threads.push_back(std::thread(incrementCounter));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "shared value: " << shared_counter << std::endl;
    return 0;
}
