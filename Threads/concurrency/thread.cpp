#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

int main() {
    std::cout << "[main] Starting multi-thread \n" << std::endl;

    auto lamda = [](int x){
        std::cout<<"hello from this thread id:"<<std::this_thread::get_id()<<std::endl;
        std::cout << "\nHello from lambda thread!" << x << std::endl;
    };
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(lamda, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\n[main] All threads completed. Done." << std::endl;
    return 0;
}
