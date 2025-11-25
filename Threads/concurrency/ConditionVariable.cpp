#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>          
#include <mutex>          
#include <condition_variable>  

std::mutex gLock;
std::condition_variable gConditionVariable;

int main(){

    int result = 0;
    bool notified = false;

    std::thread reporter([&](){  
        
        std::unique_lock<std::mutex> lock(gLock);
        if(!notified){
            // WAIT: This does 3 things atomically:
            // 1. Unlocks gLock (so worker can acquire it)
            // 2. Puts this thread to sleep (no CPU usage)
            // 3. When notified, wakes up and RE-LOCKS gLock before returning
            gConditionVariable.wait(lock);
        }   
        std::cout << "Result is: " << result << std::endl;
    });

    std::thread worker([&](){
        std::this_thread::sleep_for(std::chrono::seconds(2)); 
        {
            std::lock_guard<std::mutex> lock(gLock);
            result = 12 + 12;
            notified = true;
            std::cout << "[Worker] Work completed. Result computed." << std::endl;
        }  
        gConditionVariable.notify_one();
    });

    reporter.join();
    worker.join();    
    
    std::cout << "Main thread completed. Done." << std::endl;
    return 0;
}