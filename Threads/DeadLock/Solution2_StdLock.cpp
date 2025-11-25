#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void ThreadA() {
    // Create unique_locks WITHOUT locking (std::defer_lock)
    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
    
    // std::lock() locks BOTH atomically (deadlock-free)
    std::lock(lock1, lock2);
    
    std::cout << " Thread A Locked mtx1 and mtx2 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Locks automatically released when unique_lock goes out of scope
}

void ThreadB() {
    std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
    
    // Order doesn't matter! std::lock() handles it
    std::lock(lock2, lock1);  // Different order from ThreadA, still safe
    
    std::cout << " Thread B Locked mtx2 and mtx1 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::cout << "SOLUTION 2: std::lock() \n";
    std::cout << "Uses deadlock-avoidance algorithm internally\n\n";
    
    std::thread t1(ThreadA);
    std::thread t2(ThreadB);
    
    t1.join();
    t2.join();
    
    std::cout << "\n No deadlock! std::lock() prevented it.\n";
    return 0;
}