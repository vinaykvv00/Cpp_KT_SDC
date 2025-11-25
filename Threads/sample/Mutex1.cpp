#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
int counter = 0;
mutex mtx; 

void increment() {
    for(int i = 0; i < 10000; ++i) {
        lock_guard<mutex> lock(mtx);
        ++counter; 
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    cout << "Counter: " << counter << std::endl;
    return 0;
}

// mtx.lock();   // Lock the mutex
// // critical section (shared data)
// mtx.unlock(); // Unlock the mutex

// lock_guard: Use for simple, exception-safe locking.
// unique_lock: Use when you need to lock/unlock multiple times or defer locking.
// shared_lock/shared_mutex: Use when you have many readers but only one writer.