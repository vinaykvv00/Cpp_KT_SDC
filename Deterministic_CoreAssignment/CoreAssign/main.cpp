#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void taskOnCore(int core_id, int task_id) {

    cpu_set_t cpuset;
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    cout << "Task " << task_id << " assigned to Core " << core_id << endl;
    
    for (int i = 0; i < 5; i++) {
        cout << "Task " << task_id << " working..." << endl;
        sleep(1);
    }
    
    cout << "Task " << task_id << " finished!" << endl;
}

int main() {

    thread t1(taskOnCore, 0, 1);  
    thread t2(taskOnCore, 1, 2);  
 
    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    CPU_SET(0, &cpuset1); 
    pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t), &cpuset1);
    
    cpu_set_t cpuset2;
    CPU_ZERO(&cpuset2);
    CPU_SET(1, &cpuset2);  
    pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t), &cpuset2);
    
    t1.join();
    t2.join();
    
    cout << "All tasks completed!" << endl;
    return 0;
}