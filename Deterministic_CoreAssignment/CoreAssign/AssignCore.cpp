#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>

void backgroundWork(int task_id, int core_id) {
   
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    std::cout << "Task " << task_id << " running on Core " << core_id << std::endl;
    
    for (int i = 0; i < 5; i++) {
        std::cout << "  Task " << task_id << " - Work " << i << std::endl;
        sleep(5);
    }
    
    std::cout << "Task " << task_id << " finished on Core " << core_id << std::endl;
}

void fastCalc(int task_id, int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    std::cout << "Calculator " << task_id << " running on Core " << core_id << std::endl;
    
    long long sum = 0;
    for (int i = 0; i < 100000; i++) {  
        sum += i;
        sleep(5);
    }

    std::cout << "Calculator " << task_id << " result: " << sum << " (Core " << core_id << ")" << std::endl;
}

int main() {
    int num_cores = std::thread::hardware_concurrency();
    std::cout << "Total cores available: " << num_cores << std::endl;
    std::cout << "\nStarting 4 tasks on different cores..." << std::endl;
    
    std::thread t1(backgroundWork, 1, 0);   
    std::thread t2(backgroundWork, 2, 1);   
    std::thread t3(fastCalc, 3, 2);          
    std::thread t4(fastCalc, 4, 3);          
    
    std::cout << "\nAll tasks started! Running in parallel...\n" << std::endl;

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cout << "\nAll tasks completed!" << std::endl;
    return 0;
}