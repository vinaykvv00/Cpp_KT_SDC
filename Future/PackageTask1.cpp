#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <queue>

using namespace std::chrono_literals;

int compute_square(int x) {
    std::cout << "  Computing " << x << " * " << x << std::endl;
    std::this_thread::sleep_for(1s);
    return x * x;
}

void demo_basic_concept() {
    std::cout << "\n Basic packaged_task Concept " << std::endl;
    
    std::cout << "\n What is packaged_task " << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "It's a CLASS TEMPLATE that:" << std::endl;
    std::cout << "  1. Wraps a callable (function/lambda/functor)" << std::endl;
    std::cout << "  2. Creates a future automatically" << std::endl;
    std::cout << "  3. You control WHEN to execute it" << std::endl;
    std::cout << "  4. Result goes to the future when executed" << std::endl;
    
    std::packaged_task<int(int)> task(compute_square);
    
    std::cout << "  Task created (but NOT executed yet!)" << std::endl;
    
    std::cout << "\nStep 2: Getting future from task" << std::endl;
    std::future<int> fut = task.get_future();
    std::cout << "  Future obtained" << std::endl;
    
    std::cout << "\nStep 3: Executing the task NOW with argument 5" << std::endl;
    task(5);  
    
    std::cout << "\nStep 4: Getting result from future" << std::endl;
    int result = fut.get();
    std::cout << "  Result: " << result << std::endl;
    
    std::cout << "\nKEY POINT: We controlled WHEN the task executed!" << std::endl;
}

int main() {
    demo_basic_concept();
    return 0;
}