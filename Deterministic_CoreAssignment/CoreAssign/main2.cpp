#include <iostream>
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <atomic>
#include <csignal>

std::atomic<bool> keep_running(true);

void signalHandler(int signum) {
    keep_running = false;
} 

void counterTask(int target_core) {
 
    std::cout << "TID: " << gettid() << std::endl;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(target_core, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    int counterCore = sched_getcpu();
    std::cout << "Counter Pinned to Core " << counterCore << std::endl;

    while (keep_running) {
        for (int i = 0; i < 100000000; ++i) {}
    }
}

void calculatorTask(int target_core) {
    
    std::cout << "TID: " << gettid() << std::endl;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(target_core, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    int calculatorCore = sched_getcpu();
    std::cout << "Calculator Pinned to Core " << calculatorCore << std::endl;

    while (keep_running) {
        for (int i = 0; i < 100000000; ++i) {}
    }
}

void monitorTask(int target_core) {
   
    std::cout << "TID: " << gettid() << std::endl;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(target_core, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    int monitorCore = sched_getcpu();
    std::cout << "Monitor Pinned to Core " << monitorCore << std::endl;

    while (keep_running) {
        for (int i = 0; i < 100000000; ++i) {}
    }
}

int main() {
    std::signal(SIGINT, signalHandler);
    
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "3 threads on cores 0, 1, 2" << std::endl;
    
    std::thread t1(counterTask, 0);
    std::thread t2(calculatorTask, 1);
    std::thread t3(monitorTask, 2);
    
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
