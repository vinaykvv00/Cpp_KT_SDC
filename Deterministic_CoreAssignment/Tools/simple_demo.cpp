#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>

void burnCPU(int core_num) {
    // Pin this thread to specific core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_num, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    std::cout << "🔥 Core " << core_num << " is now BUSY!" << std::endl;
    
    // Burn CPU forever
    while(true) {
        volatile long sum = 0;
        for(int i = 0; i < 10000000; i++) {
            sum += i;
        }
    }
}

int main() {
    std::cout << "\n════════════════════════════════════════" << std::endl;
    std::cout << "  SIMPLE HTOP DEMO" << std::endl;
    std::cout << "════════════════════════════════════════\n" << std::endl;
    
    std::cout << "STEP 1: I will start 4 threads" << std::endl;
    std::cout << "STEP 2: Open htop in another terminal" << std::endl;
    std::cout << "STEP 3: Look at the TOP of htop" << std::endl;
    std::cout << "\nYou should see:" << std::endl;
    std::cout << "  Core 0: [100%] ← FULL BAR" << std::endl;
    std::cout << "  Core 1: [100%] ← FULL BAR" << std::endl;
    std::cout << "  Core 2: [100%] ← FULL BAR" << std::endl;
    std::cout << "  Core 3: [100%] ← FULL BAR" << std::endl;
    std::cout << "\nPress Ctrl+C to stop this program\n" << std::endl;
    
    std::cout << "Starting in 3 seconds..." << std::endl;
    sleep(3);
    
    // Start 4 threads on cores 0, 1, 2, 3
    std::thread t0(burnCPU, 0);
    std::thread t1(burnCPU, 1);
    std::thread t2(burnCPU, 2);
    std::thread t3(burnCPU, 3);
    
    std::cout << "\n✅ 4 threads running!" << std::endl;
    std::cout << "✅ NOW open htop and look!" << std::endl;
    std::cout << "\nPress Ctrl+C when done looking\n" << std::endl;
    
    // Wait forever
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
