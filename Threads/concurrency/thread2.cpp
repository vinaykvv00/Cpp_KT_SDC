#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

int main() {
    std::cout << "[main] Starting multi-jthread \n" << std::endl;

    auto lamda = [](int x){
        std::cout<<"hello from this thread id:"<<std::this_thread::get_id()<<std::endl;
        std::cout << "\nHello from lambda thread!" << x << std::endl;
    };
    std::vector<std::jthread> jthreads;
    for (int i = 0; i < 10; ++i) {
        jthreads.push_back(std::jthread(lamda, i));
    }

    std::cout << "\n[main] All threads completed. Done." << std::endl;
    return 0;
}
