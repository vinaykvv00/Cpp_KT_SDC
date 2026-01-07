#include <iostream>
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

void taskWithAffinityQuery(int target_core) {
    std::cout << "TID: " << gettid() << std::endl;

    cpu_set_t cpuset_set;
    CPU_ZERO(&cpuset_set);
    CPU_SET(target_core, &cpuset_set);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset_set);
    
    std::cout << "Set affinity to Core " << target_core << std::endl;

    cpu_set_t cpuset_get;
    CPU_ZERO(&cpuset_get);
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset_get);
    
    std::cout << "Querying affinity mask..." << std::endl;
    std::cout << "Allowed cores: ";
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpuset_get)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    
    int current_core = sched_getcpu();
    std::cout << "Actually running on Core " << current_core << std::endl;
    
    sleep(2);
}

int main() {
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "Step 6: Query affinity from inside code" << std::endl;
    
    std::thread t1(taskWithAffinityQuery, 0);
    std::thread t2(taskWithAffinityQuery, 5);
    std::thread t3(taskWithAffinityQuery, 10);
    
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
