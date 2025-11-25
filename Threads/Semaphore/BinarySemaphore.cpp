#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

std::binary_semaphore sem(1); 
int counter = 0;

void increment() {
    for (int i = 0; i < 1000; i++) {
        sem.acquire();
        ++counter;  
        sem.release();
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();

    std::cout << "Counter = " << counter << std::endl;
}
