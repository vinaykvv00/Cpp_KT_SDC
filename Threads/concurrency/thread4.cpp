#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>


static std::atomic<int> shared_counter = 0;

void incrementCounter() {
    shared_counter++;
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
