#include <iostream>
#include <thread>
#include <pthread.h>
#include <chrono>

void highPriorityWork(int id) {
    auto start = std::chrono::steady_clock::now();
    
    long long count = 0;
    for (int i = 0; i < 200000000; i++) {  
        count++;
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "HIGH Priority Thread " << id << " finished" << std::endl;
    std::cout << "  Count: " << count << " | Time: " << duration << "ms" << std::endl;
}

void lowPriorityWork(int id) {
    auto start = std::chrono::steady_clock::now();
    
    long long count = 0;
    for (int i = 0; i < 200000000; i++) {  
        count++;
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "LOW Priority Thread " << id << " finished" << std::endl;
    std::cout << "  Count: " << count << " | Time: " << duration << "ms" << std::endl;
}

int main() {
    std::cout << " Thread Priority" << std::endl;    

    std::thread t_high(highPriorityWork, 1);
    std::thread t_low(lowPriorityWork, 2);
    
    sched_param param_high;
    param_high.sched_priority = 99;  
    int result1 = pthread_setschedparam(t_high.native_handle(), SCHED_FIFO, &param_high);
    if (result1 == 0) {
        std::cout << " HIGH priority set successfully" << std::endl;
    } else {
        std::cout << " Failed to set HIGH priority (need root)" << std::endl;
    }
    
  
    sched_param param_low;
    param_low.sched_priority = 10; 
    int result2 = pthread_setschedparam(t_low.native_handle(), SCHED_FIFO, &param_low);
    if (result2 == 0) {
        std::cout << " LOW priority set successfully" << std::endl;
    } else {
        std::cout << " Failed to set LOW priority (need root)" << std::endl;
    }
    
    std::cout << "\nThreads running...\n" << std::endl;
    
    t_high.join();
    t_low.join();
    
    std::cout << "HIGH priority thread should finish faster!" << std::endl;
    
    return 0;
}
