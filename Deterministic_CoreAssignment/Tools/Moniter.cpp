#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>

void cpuIntensiveWork(int core_id, int task_id) {

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    std::cout << "Task " << task_id << " pinned to Core " << core_id 
              << " - PID: " << getpid() << std::endl;

    std::cout << "Task " << task_id << " working for 30 seconds..." << std::endl;
    
    for (int sec = 0; sec < 30; sec++) {

        long long sum = 0;
        for (long long i = 0; i < 500000000; i++) {
            sum += i;
        }
        std::cout << "Task " << task_id << " on Core " << core_id 
                  << " - Second " << (sec + 1) << "/30" << std::endl;
    }
    
    std::cout << "Task " << task_id << " finished!" << std::endl;
}

int main() {

    std::cout << "\nStarting 4 threads on 4 different cores...\n" << std::endl;
    
    sleep(3); 

    std::thread t0(cpuIntensiveWork, 0, 1);
    std::thread t1(cpuIntensiveWork, 1, 2);
    std::thread t2(cpuIntensiveWork, 2, 3);
    std::thread t3(cpuIntensiveWork, 3, 4);
    
    std::cout << "\n all threads running!" << std::endl;
    
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    
    std::cout << "\n All tasks completed!" << std::endl;
    
    return 0;
}