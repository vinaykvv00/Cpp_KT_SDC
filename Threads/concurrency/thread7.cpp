#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <mutex>

std::mutex mtx;
std::condition_variable cv;
static int count = 0;

void Odd_thread() {
    while (count < 9) {
        std::unique_lock<std::mutex> lock(mtx);
        while (count % 2 != 1) {  
                cv.wait(lock);       
        }
        count++;
        std::cout << count << std::endl;
        lock.unlock();
        cv.notify_all();  
    }
}

void Even_thread () {
    while (count < 9) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return count % 2 == 0; }); 
        count++;
        std::cout << count << std::endl;
        lock.unlock();
        cv.notify_all();  
    }
}
int main()
{
    std::thread oddThread(Odd_thread);
    std::thread evenThread(Even_thread);

    oddThread.join();
    evenThread.join();

    return 0;
}
