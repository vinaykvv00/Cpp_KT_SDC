/**
 * @file 06_packaged_task_complete.cpp
 * @brief Complete demonstration of std::packaged_task
 * 
 * WHAT IS std::packaged_task?
 * ===========================
 * - A CLASS TEMPLATE (not a function or keyword)
 * - Defined in <future> header
 * - Wraps any callable object (function, lambda, functor)
 * - Automatically creates a std::future for the result
 * - Combines the functionality of a callable + std::promise
 * - Allows delayed or deferred execution
 * 
 * THINK OF IT AS:
 * ==============
 * A "task package" that you can:
 * 1. Create with a function
 * 2. Get a future for the result
 * 3. Execute later (manually control when it runs)
 * 4. Pass to threads or call directly
 * 
 * COMPARISON:
 * ===========
 * std::async       = Automatic execution (high-level)
 * std::promise     = Manual value setting (low-level)
 * std::packaged_task = Manual execution of function (medium-level)
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <queue>

using namespace std::chrono_literals;

// ============================================================================
// DEMONSTRATION 1: Basic packaged_task Concept
// ============================================================================

int compute_square(int x) {
    std::cout << "  Computing " << x << " * " << x << std::endl;
    std::this_thread::sleep_for(1s);
    return x * x;
}

void demo_basic_concept() {
    std::cout << "\n=== DEMO 1: Basic packaged_task Concept ===" << std::endl;
    
    std::cout << "\nUNDERSTANDING: What is packaged_task?" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "It's a CLASS TEMPLATE that:" << std::endl;
    std::cout << "  1. Wraps a callable (function/lambda/functor)" << std::endl;
    std::cout << "  2. Creates a future automatically" << std::endl;
    std::cout << "  3. You control WHEN to execute it" << std::endl;
    std::cout << "  4. Result goes to the future when executed" << std::endl;
    
    // Step 1: Create a packaged_task
    std::cout << "\nStep 1: Creating packaged_task<int(int)>" << std::endl;
    std::packaged_task<int(int)> task(compute_square);
    //                  ↑     ↑
    //                  |     └─ Parameter type(s)
    //                  └─ Return type
    
    std::cout << "  Task created (but NOT executed yet!)" << std::endl;
    
    // Step 2: Get the future
    std::cout << "\nStep 2: Getting future from task" << std::endl;
    std::future<int> fut = task.get_future();
    std::cout << "  Future obtained" << std::endl;
    
    // Step 3: Execute the task (you control when!)
    std::cout << "\nStep 3: Executing the task NOW with argument 5" << std::endl;
    task(5);  // This is when the function actually runs!
    //   ↑
    //   └─ Pass arguments here
    
    // Step 4: Get the result
    std::cout << "\nStep 4: Getting result from future" << std::endl;
    int result = fut.get();
    std::cout << "  Result: " << result << std::endl;
    
    std::cout << "\nKEY POINT: We controlled WHEN the task executed!" << std::endl;
}

// ============================================================================
// DEMONSTRATION 2: packaged_task Template Signature
// ============================================================================

void demo_template_signature() {
    std::cout << "\n=== DEMO 2: Understanding Template Signature ===" << std::endl;
    
    std::cout << "\nTEMPLATE FORMAT: std::packaged_task<ReturnType(Args...)>" << std::endl;
    std::cout << "========================================================" << std::endl;
    
    // Example 1: Function returning int, taking int
    std::cout << "\n1. packaged_task<int(int)>" << std::endl;
    std::packaged_task<int(int)> task1([](int x) { return x * 2; });
    std::cout << "   Return: int, Parameter: int" << std::endl;
    
    // Example 2: Function returning void, taking no parameters
    std::cout << "\n2. packaged_task<void()>" << std::endl;
    std::packaged_task<void()> task2([]() { 
        std::cout << "   Executing void task with no parameters" << std::endl; 
    });
    std::cout << "   Return: void, Parameters: none" << std::endl;
    
    // Example 3: Function returning double, taking int and double
    std::cout << "\n3. packaged_task<double(int, double)>" << std::endl;
    std::packaged_task<double(int, double)> task3([](int a, double b) { 
        return a + b; 
    });
    std::cout << "   Return: double, Parameters: int, double" << std::endl;
    
    // Example 4: Function returning string, taking string
    std::cout << "\n4. packaged_task<std::string(std::string)>" << std::endl;
    std::packaged_task<std::string(std::string)> task4([](std::string s) { 
        return "Hello, " + s; 
    });
    std::cout << "   Return: string, Parameter: string" << std::endl;
    
    std::cout << "\nEXECUTING EXAMPLES:" << std::endl;
    
    auto fut1 = task1.get_future();
    task1(5);
    std::cout << "  task1(5) result: " << fut1.get() << std::endl;
    
    auto fut2 = task2.get_future();
    task2();  // No arguments
    fut2.get();  // Returns void
    
    auto fut3 = task3.get_future();
    task3(10, 3.14);
    std::cout << "  task3(10, 3.14) result: " << fut3.get() << std::endl;
    
    auto fut4 = task4.get_future();
    task4("World");
    std::cout << "  task4(\"World\") result: " << fut4.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 3: All Member Functions of packaged_task
// ============================================================================

void demo_member_functions() {
    std::cout << "\n=== DEMO 3: All packaged_task Member Functions ===" << std::endl;
    
    // 1. Constructor - Create from callable
    std::cout << "\n1. Constructor:" << std::endl;
    std::packaged_task<int(int)> task1([](int x) { return x * x; });
    std::cout << "   Created from lambda" << std::endl;
    
    // 2. get_future() - Get the associated future (once only!)
    std::cout << "\n2. get_future():" << std::endl;
    std::future<int> fut1 = task1.get_future();
    std::cout << "   Future obtained (can only call once per task)" << std::endl;
    
    // 3. operator() - Execute the task
    std::cout << "\n3. operator() - Execute the task:" << std::endl;
    task1(7);  // Executes the wrapped function
    std::cout << "   Task executed with argument 7" << std::endl;
    std::cout << "   Result: " << fut1.get() << std::endl;
    
    // 4. valid() - Check if task has a shared state
    std::cout << "\n4. valid() - Check if task is valid:" << std::endl;
    std::packaged_task<int(int)> task2([](int x) { return x + 10; });
    std::cout << "   Before get_future(), valid: " << std::boolalpha << task2.valid() << std::endl;
    auto fut2 = task2.get_future();
    std::cout << "   After get_future(), valid: " << task2.valid() << std::endl;
    task2(5);
    std::cout << "   After execution, valid: " << task2.valid() << std::endl;
    std::cout << "   Result: " << fut2.get() << std::endl;
    
    // 5. reset() - Reset the task (get new shared state)
    std::cout << "\n5. reset() - Reuse the task:" << std::endl;
    std::packaged_task<int(int)> task3([](int x) { return x * 100; });
    
    // First use
    auto fut3a = task3.get_future();
    task3(2);
    std::cout << "   First execution: " << fut3a.get() << std::endl;
    
    // Reset and reuse
    task3.reset();  // Get new shared state
    std::cout << "   Task reset! Can use again." << std::endl;
    
    auto fut3b = task3.get_future();
    task3(3);
    std::cout << "   Second execution: " << fut3b.get() << std::endl;
    
    // 6. swap() - Swap two tasks
    std::cout << "\n6. swap() - Swap two tasks:" << std::endl;
    std::packaged_task<int(int)> taskA([](int x) { return x + 1; });
    std::packaged_task<int(int)> taskB([](int x) { return x + 2; });
    
    auto futA = taskA.get_future();
    auto futB = taskB.get_future();
    
    std::cout << "   Before swap:" << std::endl;
    taskA.swap(taskB);  // Swap the tasks
    std::cout << "   After swap:" << std::endl;
    
    taskA(10);  // This now executes the original taskB function
    taskB(10);  // This now executes the original taskA function
    
    std::cout << "   taskA result: " << futA.get() << std::endl;  // Should be 12 (10+2)
    std::cout << "   taskB result: " << futB.get() << std::endl;  // Should be 11 (10+1)
    
    // 7. make_ready_at_thread_exit() - Set result when thread exits
    std::cout << "\n7. make_ready_at_thread_exit():" << std::endl;
    std::packaged_task<int(int)> task4([](int x) { return x * x; });
    auto fut4 = task4.get_future();
    
    std::thread t([&task4]() {
        std::cout << "   [Thread] Task executing..." << std::endl;
        task4.make_ready_at_thread_exit(5);
        std::cout << "   [Thread] Result will be ready at thread exit" << std::endl;
        std::this_thread::sleep_for(1s);
        std::cout << "   [Thread] Thread exiting now..." << std::endl;
    });
    
    std::cout << "   [Main] Waiting for result..." << std::endl;
    int result = fut4.get();  // Blocks until thread exits
    std::cout << "   [Main] Got result: " << result << std::endl;
    t.join();
}

// ============================================================================
// DEMONSTRATION 4: Comparison with async and promise
// ============================================================================

int expensive_computation(int x) {
    std::this_thread::sleep_for(500ms);
    return x * x;
}

void demo_comparison() {
    std::cout << "\n=== DEMO 4: Comparison: async vs promise vs packaged_task ===" << std::endl;

    std::cout << "\n1. Using std::async:" << std::endl;
    std::cout << "   - Automatic execution" << std::endl;
    std::cout << "   - Creates thread automatically (with async policy)" << std::endl;
    std::cout << "   - No manual control over execution timing" << std::endl;
    
    auto fut_async = std::async(std::launch::async, expensive_computation, 5);
    std::cout << "   Task ALREADY running in background!" << std::endl;
    std::cout << "   Result: " << fut_async.get() << std::endl;
    
    // METHOD 2: Using std::promise (manual value setting)
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
    
    // METHOD 3: Using std::packaged_task (manual execution control)
    std::cout << "\n3. Using std::packaged_task:" << std::endl;
    std::cout << "   - Wraps a function" << std::endl;
    std::cout << "   - You control WHEN it executes" << std::endl;
    std::cout << "   - Can pass to threads or call directly" << std::endl;
    
    std::packaged_task<int(int)> task(expensive_computation);
    auto fut_task = task.get_future();
    
    std::cout << "   Task created but NOT running yet..." << std::endl;
    std::cout << "   Deciding to run it in a thread..." << std::endl;
    
    std::thread t2(std::move(task), 5);  // Pass task to thread
    
    std::cout << "   Now task is executing in thread!" << std::endl;
    std::cout << "   Result: " << fut_task.get() << std::endl;
    t2.join();
    
    std::cout << "\nSUMMARY:" << std::endl;
    std::cout << "  async:         Automatic (high-level)" << std::endl;
    std::cout << "  promise:       Manual value (low-level)" << std::endl;
    std::cout << "  packaged_task: Manual execution (medium-level)" << std::endl;
}

// ============================================================================
// DEMONSTRATION 5: Different Callable Types with packaged_task
// ============================================================================

// Regular function
int add(int a, int b) {
    return a + b;
}

// Function object (functor)
struct Multiplier {
    int factor;
    Multiplier(int f) : factor(f) {}
    
    int operator()(int x) const {
        return x * factor;
    }
};

// Class with member function
class Calculator {
public:
    int value;
    Calculator(int v) : value(v) {}
    
    int compute(int x) const {
        return value + x;
    }
};

void demo_callable_types() {
    std::cout << "\n=== DEMO 5: Different Callable Types ===" << std::endl;
    
    // 1. Regular function
    std::cout << "\n1. Regular function:" << std::endl;
    std::packaged_task<int(int, int)> task1(add);
    auto fut1 = task1.get_future();
    task1(10, 20);
    std::cout << "   add(10, 20) = " << fut1.get() << std::endl;
    
    // 2. Lambda function
    std::cout << "\n2. Lambda function:" << std::endl;
    std::packaged_task<int(int)> task2([](int x) { return x * x; });
    auto fut2 = task2.get_future();
    task2(7);
    std::cout << "   lambda(7) = " << fut2.get() << std::endl;
    
    // 3. Lambda with capture
    std::cout << "\n3. Lambda with capture:" << std::endl;
    int multiplier = 5;
    std::packaged_task<int(int)> task3([multiplier](int x) { return x * multiplier; });
    auto fut3 = task3.get_future();
    task3(8);
    std::cout << "   captured_lambda(8) = " << fut3.get() << std::endl;
    
    // 4. Function object (functor)
    std::cout << "\n4. Function object (functor):" << std::endl;
    Multiplier mult(3);
    std::packaged_task<int(int)> task4(mult);
    auto fut4 = task4.get_future();
    task4(9);
    std::cout << "   functor(9) = " << fut4.get() << std::endl;
    
    // 5. std::function
    std::cout << "\n5. std::function:" << std::endl;
    std::function<int(int, int)> func = [](int a, int b) { return a - b; };
    std::packaged_task<int(int, int)> task5(func);
    auto fut5 = task5.get_future();
    task5(100, 30);
    std::cout << "   std::function(100, 30) = " << fut5.get() << std::endl;
    
    // 6. std::bind (for member functions)
    std::cout << "\n6. Member function with std::bind:" << std::endl;
    Calculator calc(50);
    auto bound_func = std::bind(&Calculator::compute, &calc, std::placeholders::_1);
    std::packaged_task<int(int)> task6(bound_func);
    auto fut6 = task6.get_future();
    task6(25);
    std::cout << "   member_function(25) = " << fut6.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 6: Real-World Use Case - Task Queue
// ============================================================================

class TaskQueue {
private:
    std::queue<std::packaged_task<void()>> tasks;
    std::mutex mtx;
    bool running = true;
    
public:
    // Add a task to the queue
    template<typename Func, typename... Args>
    auto enqueue(Func&& f, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
        using return_type = typename std::result_of<Func(Args...)>::type;
        
        // Create packaged_task
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> result = task->get_future();
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            
            // Wrap in void() task for queue
            tasks.emplace([task]() { (*task)(); });
        }
        
        return result;
    }
    
    // Worker thread processes tasks
    void worker() {
        while (running) {
            std::packaged_task<void()> task;
            
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (!tasks.empty()) {
                    task = std::move(tasks.front());
                    tasks.pop();
                } else {
                    std::this_thread::sleep_for(100ms);
                    continue;
                }
            }
            
            if (task.valid()) {
                task();  // Execute the task
            }
        }
    }
    
    void stop() {
        running = false;
    }
};

int task_work(int id, int value) {
    std::cout << "  [Task " << id << "] Processing value: " << value << std::endl;
    std::this_thread::sleep_for(300ms);
    return value * value;
}

void demo_task_queue() {
    std::cout << "\n=== DEMO 6: Real-World Use Case - Task Queue ===" << std::endl;
    std::cout << "\nCreating task queue with worker thread..." << std::endl;
    
    TaskQueue queue;
    std::thread worker([&queue]() { queue.worker(); });
    
    // Enqueue multiple tasks
    std::cout << "\nEnqueuing tasks..." << std::endl;
    auto fut1 = queue.enqueue(task_work, 1, 5);
    auto fut2 = queue.enqueue(task_work, 2, 7);
    auto fut3 = queue.enqueue(task_work, 3, 9);
    
    std::cout << "Tasks enqueued, worker processing..." << std::endl;
    
    // Get results
    std::cout << "\nCollecting results:" << std::endl;
    std::cout << "  Task 1 result: " << fut1.get() << std::endl;
    std::cout << "  Task 2 result: " << fut2.get() << std::endl;
    std::cout << "  Task 3 result: " << fut3.get() << std::endl;
    
    queue.stop();
    worker.join();
    std::cout << "\nTask queue completed!" << std::endl;
}

// ============================================================================
// DEMONSTRATION 7: Exception Handling
// ============================================================================

int risky_task(int value) {
    if (value < 0) {
        throw std::invalid_argument("Negative value not allowed!");
    }
    return value * 10;
}

void demo_exception_handling() {
    std::cout << "\n=== DEMO 7: Exception Handling ===" << std::endl;
    
    // Test 1: Successful execution
    std::cout << "\n1. Successful execution:" << std::endl;
    std::packaged_task<int(int)> task1(risky_task);
    auto fut1 = task1.get_future();
    task1(5);
    try {
        int result = fut1.get();
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   Exception: " << e.what() << std::endl;
    }
    
    // Test 2: Exception thrown
    std::cout << "\n2. Exception in task:" << std::endl;
    std::packaged_task<int(int)> task2(risky_task);
    auto fut2 = task2.get_future();
    task2(-5);  // This will throw
    try {
        int result = fut2.get();  // Exception re-thrown here
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "   Caught exception: " << e.what() << std::endl;
    }
    
    // Test 3: Exception in thread
    std::cout << "\n3. Exception in thread:" << std::endl;
    std::packaged_task<int(int)> task3(risky_task);
    auto fut3 = task3.get_future();
    
    std::thread t([&task3]() {
        task3(-10);  // Throws in thread
    });
    
    try {
        int result = fut3.get();  // Exception propagated to main thread
    } catch (const std::invalid_argument& e) {
        std::cout << "   Exception from thread: " << e.what() << std::endl;
    }
    
    t.join();
}

// ============================================================================
// DEMONSTRATION 8: Move Semantics
// ============================================================================

void demo_move_semantics() {
    std::cout << "\n=== DEMO 8: Move Semantics ===" << std::endl;
    
    std::cout << "\npackaged_task is MOVE-ONLY (like future and promise)" << std::endl;
    
    std::packaged_task<int(int)> task1([](int x) { return x * 2; });
    std::cout << "task1 valid: " << std::boolalpha << task1.valid() << std::endl;
    
    // Move task
    std::packaged_task<int(int)> task2 = std::move(task1);
    std::cout << "\nAfter move:" << std::endl;
    std::cout << "  task1 valid: " << task1.valid() << std::endl;  // false
    std::cout << "  task2 valid: " << task2.valid() << std::endl;  // true
    
    // Move to thread
    std::cout << "\nMoving task to thread:" << std::endl;
    auto fut = task2.get_future();
    std::thread t(std::move(task2), 10);  // task2 moved to thread
    
    std::cout << "  task2 valid after move to thread: " << task2.valid() << std::endl;  // false
    std::cout << "  Result: " << fut.get() << std::endl;
    t.join();
    
    // UNCOMMENT TO SEE ERROR:
    // std::packaged_task<int(int)> task3 = task2;  // ERROR: not copyable
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║       STD::PACKAGED_TASK COMPLETE GUIDE                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_basic_concept();
    demo_template_signature();
    demo_member_functions();
    demo_comparison();
    demo_callable_types();
    demo_task_queue();
    demo_exception_handling();
    demo_move_semantics();
    
    std::cout << "\n✓ All std::packaged_task demonstrations completed!" << std::endl;
    return 0;
}
