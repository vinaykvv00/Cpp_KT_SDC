#include <iostream>
#include <future>
#include <thread>   // Required for std::this_thread::sleep_for and std::this_thread::get_id
#include <chrono>

int compute(int x) {
    std::cout << "Computing square of " << x << " in thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x * x;
}

int cubeThread(int x) {
    std::cout << "Computing cube of " << x << " in thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x * x * x;
}

int main() {
    std::future<int> computedThread = std::async(std::launch::async, compute, 5);
    std::cout << "Computing square of 5 asynchronously..." << std::endl;

    std::cout << "Result: " << computedThread.get() << std::endl;  
    
    std::cout << "Computing sum of 10 and 20 asynchronously..." << std::endl;
    auto fut = std::async(std::launch::async, [](int a, int b)
    {
      return a + b;
    }, 10, 20);

    std::cout << "Computing cube of 3 asynchronously..." << std::endl;
    auto fut2 = std::async(cubeThread, 3);

    std::cout << "Sum: " << fut.get() << std::endl; 
    std::cout << fut2.get() << std::endl;

    std::cout<<"main thread ending."<<std::endl;

    return 0;
}