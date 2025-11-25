#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void ThreadA() {
    int attempts = 0;
    
    while (true) {

        std::unique_lock<std::mutex> lock1(mtx1);

        if (mtx2.try_lock()) {
            std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);  
            std::cout << "[Thread A] Locked both mutexes  (attempts: " << attempts << ")\n";
            break;
        }
        lock1.unlock();
        ++attempts;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ThreadB() {
    int attempts = 0;
    
    while (true) {
        std::unique_lock<std::mutex> lock2(mtx2);
        
        if (mtx1.try_lock()) {
            std::unique_lock<std::mutex> lock1(mtx1, std::adopt_lock);
            std::cout << "[Thread B] Locked both mutexes  (attempts: " << attempts << ")\n";
            break;
        }
        
        lock2.unlock();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::cout << " SOLUTION 4: try_lock(): \n";
    std::cout << "Non-blocking approach with retry logic\n\n";
    
    std::thread t1(ThreadA);
    std::thread t2(ThreadB);
    
    t1.join();
    t2.join();
    
    std::cout << "\n No deadlock! Retry mechanism worked.\n";
    return 0;
}

/*
Execution Example:
  Time    Thread A                    Thread B
  ----    --------                    --------
  0ms     lock(mtx1) ✓                lock(mtx2) ✓
  1ms     try_lock(mtx2) → FAILS      try_lock(mtx1) → FAILS
  2ms     unlock(mtx1)                unlock(mtx2)
  10ms    sleep...                    sleep...
  20ms    lock(mtx1) ✓                lock(mtx2) ✓
  21ms    try_lock(mtx2) ✓            (A wins, B waits)
  22ms    SUCCESS!                    
  23ms                                lock(mtx2) ✓
  24ms                                try_lock(mtx1) ✓
  25ms                                SUCCESS!

*/
