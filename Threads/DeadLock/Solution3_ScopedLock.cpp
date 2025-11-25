#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void ThreadA() {
    std::scoped_lock lock(mtx1, mtx2);
    
    std::cout << "[Thread A] Locked mtx1 and mtx2 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void ThreadB() {
    std::scoped_lock lock(mtx2, mtx1);
    
    std::cout << "[Thread B] Locked mtx2 and mtx1 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::cout << " SOLUTION 3: std::scoped_lock (C++17) \n";
    std::cout << "Simplest and safest solution for multiple mutexes\n\n";
    
    std::thread t1(ThreadA);
    std::thread t2(ThreadB);
    
    t1.join();
    t2.join();
    
    std::cout << "\n No deadlock! scoped_lock handles everything.\n";
    return 0;
}