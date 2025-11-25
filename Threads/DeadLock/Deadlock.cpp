#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void ThreadA() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "Thread A locked mtx1\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate delay

    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "Thread A locked mtx2\n";
}

void ThreadB() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::cout << "Thread B locked mtx2\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::lock_guard<std::mutex> lock1(mtx1);
    std::cout << "Thread B locked mtx1\n";
}

int main() {
    std::thread t1(ThreadA);
    std::thread t2(ThreadB);

    t1.join();
    t2.join();

    std::cout << "Done!\n";
    return 0;
}
