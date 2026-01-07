#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

int main() {
   
    int num_cores = thread::hardware_concurrency();
    cout << "Number of cores: " << num_cores << endl;
    
    cpu_set_t cpuset;
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    cout << "Main thread can run on cores: ";
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpuset)) {
            cout << i << " ";
        }
    }
    cout << endl;
    
    return 0;
}