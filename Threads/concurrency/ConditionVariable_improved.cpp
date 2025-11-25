#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

std::mutex gLock;
std::condition_variable gConditionVariable;

int main(){
    int result = 0;
    bool notified = false;

    //--------------------------------------------------------------
    // REPORTER THREAD - Waits for result
    //--------------------------------------------------------------
    std::thread reporter([&](){
        std::unique_lock<std::mutex> lock(gLock);
        
        // Predicate-based wait: automatically handles spurious wakeups
        // Equivalent to: while(!notified) { cv.wait(lock); }
        gConditionVariable.wait(lock, [&]{ return notified; });
        
        std::cout << "[Reporter] Result is: " << result << std::endl;
    });

    //--------------------------------------------------------------
    // WORKER THREAD - Computes result and notifies
    //--------------------------------------------------------------
    std::thread worker([&](){
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        {
            std::lock_guard<std::mutex> lock(gLock);
            result = 12 + 12;
            notified = true;
            std::cout << "[Worker] Work completed. Result computed." << std::endl;
        }  // Unlock BEFORE notify (best practice)
        
        // Notify reporter that work is done
        gConditionVariable.notify_one();
    });

    reporter.join();
    worker.join();
    std::cout << "[Main] All threads completed." << std::endl;
    return 0;
}
