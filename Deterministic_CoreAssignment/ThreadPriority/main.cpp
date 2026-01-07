#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>

void highPriorityTask() {
    std::cout << "HIGH PRIORITY task starting..." << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "HIGH: Working " << i << std::endl;
        sleep(1);
    }
    std::cout << "HIGH PRIORITY task done!" << std::endl;
}

void lowPriorityTask() {
    std::cout << "LOW PRIORITY task starting..." << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "LOW: Working " << i << std::endl;
        sleep(1);
    }
    std::cout << "LOW PRIORITY task done!" << std::endl;
}

int main() {

    std::thread t_high(highPriorityTask);
    std::thread t_low(lowPriorityTask);
    
    sched_param param_high;
    param_high.sched_priority = 50;  
    pthread_setschedparam(t_high.native_handle(), SCHED_FIFO, &param_high);
    
    sched_param param_low;
    param_low.sched_priority = 10; 
    pthread_setschedparam(t_low.native_handle(), SCHED_FIFO, &param_low);
    
    t_high.join();
    t_low.join();
    
    std::cout << "All tasks completed!" << std::endl;
    return 0;
}