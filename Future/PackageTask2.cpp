#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <queue>

int expensive_computation(int x)
{
    return x ^ 4;
}

void demo_comparison() {
    std::cout << "\n1. Using std::async:" << std::endl;
    std::cout << "   - Automatic execution" << std::endl;
    std::cout << "   - Creates thread automatically (with async policy)" << std::endl;
    std::cout << "   - No manual control over execution timing" << std::endl;
    
    auto fut_async = std::async(std::launch::async, expensive_computation, 5);
    std::cout << "   Task ALREADY running in background!" << std::endl;
    std::cout << "   Result: " << fut_async.get() << std::endl;

    std::cout << "\n2. Using std::promise:" << std::endl;
    std::cout << "   - Manual value setting" << std::endl;
    std::cout << "   - No function wrapping" << std::endl;
    std::cout << "   - You set the value explicitly" << std::endl;
    
    std::promise<int> prom;
    auto fut_promise = prom.get_future();
    
    std::thread t1([](std::promise<int> p, int x) {
        int result = expensive_computation(x);
        p.set_value(result); 
    }, std::move(prom), 5);
    
    std::cout << "   Waiting for manual value setting..." << std::endl;
    std::cout << "   Result: " << fut_promise.get() << std::endl;
    t1.join();
    

    std::cout << "\n3. Using std::packaged_task:" << std::endl;
    std::cout << "   - Wraps a function" << std::endl;
    std::cout << "   - You control WHEN it executes" << std::endl;
    std::cout << "   - Can pass to threads or call directly" << std::endl;
    
    std::packaged_task<int(int)> task(expensive_computation);
    auto fut_task = task.get_future();
    
    std::cout << "   Task created but NOT running yet..." << std::endl;
    std::cout << "   Deciding to run it in a thread..." << std::endl;
    
    std::thread t2(std::move(task), 5);  
    
    std::cout << "   Now task is executing in thread!" << std::endl;
    std::cout << "   Result: " << fut_task.get() << std::endl;
    t2.join();
    
    std::cout << "\nSUMMARY:" << std::endl;
    std::cout << "  async:         Automatic (high-level)" << std::endl;
    std::cout << "  promise:       Manual value (low-level)" << std::endl;
    std::cout << "  packaged_task: Manual execution (medium-level)" << std::endl;
}

int main()
{
    demo_comparison();
    return 0;
}