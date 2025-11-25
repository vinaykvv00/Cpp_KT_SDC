#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void ThreadA() {
    
    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "[Thread A] Locked mtx1\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "[Thread A] Locked mtx2  SUCCESS\n";
}

void ThreadB() {

    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "[Thread B] Locked mtx1\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "[Thread B] Locked mtx2  SUCCESS\n";
}

int main() {
    std::cout << " SOLUTION 1: Lock Ordering \n";
    std::cout << "Rule: All threads lock mutexes in the SAME order\n\n";
    
    std::thread t1(ThreadA);
    std::thread t2(ThreadB);
    
    t1.join();
    t2.join();
    
    std::cout << "\n  No deadlock! Both threads completed.\n";
    return 0;
}