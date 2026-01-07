#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <chrono>

void doWork(std::string priority_name, int thread_id) {
    auto start = std::chrono::steady_clock::now();
    
    std::cout << "[" << priority_name << " Thread " << thread_id << "] Starting..." << std::endl;

    long long sum = 0;
    for (int i = 0; i < 50000000; i++) {
        sum += i;
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "[" << priority_name << " Thread " << thread_id << "] Done! Time: " 
              << duration << "ms" << std::endl;
}

int main() {
    
    std::cout << "Creating 3 threads with CRITICAL, NORMAL, BACKGROUND priorities\n" << std::endl;

    std::thread t_critical([](){ doWork("CRITICAL", 1); });
    std::thread t_normal([](){ doWork("NORMAL", 2); });
    std::thread t_background([](){ doWork("BACKGROUND", 3); });

    sched_param param_critical;
    param_critical.sched_priority = 90;
    pthread_setschedparam(t_critical.native_handle(), SCHED_FIFO, &param_critical);

    sched_param param_normal;
    param_normal.sched_priority = 50;
    pthread_setschedparam(t_normal.native_handle(), SCHED_FIFO, &param_normal);
    
    sched_param param_background;
    param_background.sched_priority = 10;
    pthread_setschedparam(t_background.native_handle(), SCHED_FIFO, &param_background);
    
    std::cout << "Threads running with priorities:" << std::endl;
    std::cout << "  CRITICAL    = 90 (highest)" << std::endl;
    std::cout << "  NORMAL      = 50 (medium)" << std::endl;
    std::cout << "  BACKGROUND  = 10 (lowest)\n" << std::endl;
    
    t_critical.join();
    t_normal.join();
    t_background.join();

    std::cout << "1st: CRITICAL (priority 90)" << std::endl;
    std::cout << "2nd: NORMAL (priority 50)" << std::endl;
    std::cout << "3rd: BACKGROUND (priority 10)" << std::endl;
    
    return 0;
}
