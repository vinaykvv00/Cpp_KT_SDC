/**
 * @file 04_promise_complete.cpp
 * @brief Complete demonstration of std::promise
 * 
 * WHAT IS std::promise?
 * - A CLASS TEMPLATE (not a function or keyword)
 * - Defined in <future> header
 * - Provides a way to SET a value or exception
 * - Acts as the "sender" or "producer" side
 * - Creates a std::future for the "receiver" or "consumer" side
 * - Lower-level primitive for custom asynchronous operations
 * 
 * MEMBER FUNCTIONS:
 * - Constructor/Destructor
 * - get_future() - Get the associated future
 * - set_value() - Set the result value
 * - set_value_at_thread_exit() - Set value when thread exits
 * - set_exception() - Set an exception
 * - set_exception_at_thread_exit() - Set exception when thread exits
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// DEMONSTRATION 1: Basic Promise Usage
// ============================================================================

void demo_basic_promise() {
    std::cout << "\n=== DEMO 1: Basic Promise Usage ===" << std::endl;
    
    // 1. Create a promise
    std::promise<int> prom;
    std::cout << "Promise created" << std::endl;
    
    // 2. Get the associated future
    std::future<int> fut = prom.get_future();
    std::cout << "Future obtained from promise" << std::endl;
    
    // 3. Launch a thread that will set the value
    std::thread worker([&prom]() {
        std::cout << "  [Worker] Starting computation..." << std::endl;
        std::this_thread::sleep_for(2s);
        
        // Set the value
        prom.set_value(42);
        std::cout << "  [Worker] Value set!" << std::endl;
    });
    
    // 4. Main thread waits for the result
    std::cout << "[Main] Waiting for result..." << std::endl;
    int result = fut.get();  // Blocks until promise.set_value() is called
    std::cout << "[Main] Result received: " << result << std::endl;
    
    worker.join();
}

// ============================================================================
// DEMONSTRATION 2: Promise Constructor and get_future()
// ============================================================================

void demo_constructor_get_future() {
    std::cout << "\n=== DEMO 2: Constructor & get_future() ===" << std::endl;
    
    // 1. Default constructor
    std::promise<int> prom1;
    std::cout << "1. Default constructor - promise created" << std::endl;
    
    // 2. Get future (can only be called ONCE per promise)
    std::future<int> fut1 = prom1.get_future();
    std::cout << "   Future obtained, valid: " << std::boolalpha << fut1.valid() << std::endl;
    
    // 3. Trying to get future again throws exception
    try {
        std::future<int> fut2 = prom1.get_future();  // ERROR!
    } catch (const std::future_error& e) {
        std::cout << "   Cannot call get_future() twice: " << e.what() << std::endl;
    }
    
    // 4. Promise is move-only (not copyable)
    std::cout << "\n2. Move semantics:" << std::endl;
    std::promise<int> prom2;
    std::future<int> fut3 = prom2.get_future();
    
    // Move the promise
    std::promise<int> prom3 = std::move(prom2);
    std::cout << "   Promise moved successfully" << std::endl;
    
    // Set value through moved promise
    prom3.set_value(100);
    std::cout << "   Result: " << fut3.get() << std::endl;
    
    // UNCOMMENT TO SEE ERROR: promises cannot be copied
    // std::promise<int> prom4 = prom3;  // ERROR: promise is not copyable
}

// ============================================================================
// DEMONSTRATION 3: set_value() - Different Types
// ============================================================================

void demo_set_value() {
    std::cout << "\n=== DEMO 3: set_value() with Different Types ===" << std::endl;
    
    // 1. Set integer value
    std::cout << "\n1. Setting integer value:" << std::endl;
    std::promise<int> prom1;
    std::future<int> fut1 = prom1.get_future();
    
    std::thread t1([](std::promise<int> p) {
        std::this_thread::sleep_for(500ms);
        p.set_value(42);
        std::cout << "   Integer value set" << std::endl;
    }, std::move(prom1));
    
    std::cout << "   Result: " << fut1.get() << std::endl;
    t1.join();
    
    // 2. Set string value
    std::cout << "\n2. Setting string value:" << std::endl;
    std::promise<std::string> prom2;
    std::future<std::string> fut2 = prom2.get_future();
    
    std::thread t2([](std::promise<std::string> p) {
        std::this_thread::sleep_for(500ms);
        p.set_value("Hello from promise!");
        std::cout << "   String value set" << std::endl;
    }, std::move(prom2));
    
    std::cout << "   Result: " << fut2.get() << std::endl;
    t2.join();
    
    // 3. Set void (promise<void> specialization)
    std::cout << "\n3. Setting void (completion signal):" << std::endl;
    std::promise<void> prom3;
    std::future<void> fut3 = prom3.get_future();
    
    std::thread t3([](std::promise<void> p) {
        std::cout << "   Worker doing work..." << std::endl;
        std::this_thread::sleep_for(500ms);
        p.set_value();  // Just signals completion
        std::cout << "   Work done, signal sent" << std::endl;
    }, std::move(prom3));
    
    fut3.get();  // Wait for completion
    std::cout << "   Completion confirmed" << std::endl;
    t3.join();
    
    // 4. Set complex object
    std::cout << "\n4. Setting complex object:" << std::endl;
    
    struct Data {
        int id;
        std::string name;
        Data(int i, std::string n) : id(i), name(std::move(n)) {
            std::cout << "   Data object constructed: " << id << ", " << name << std::endl;
        }
    };
    
    std::promise<Data> prom4;
    std::future<Data> fut4 = prom4.get_future();
    
    std::thread t4([](std::promise<Data> p) {
        std::this_thread::sleep_for(500ms);
        p.set_value(Data(123, "TestData"));
    }, std::move(prom4));
    
    Data result = fut4.get();
    std::cout << "   Retrieved: " << result.id << ", " << result.name << std::endl;
    t4.join();
    
    // 5. Can only set value ONCE
    std::cout << "\n5. set_value() can only be called once:" << std::endl;
    std::promise<int> prom5;
    std::future<int> fut5 = prom5.get_future();
    
    prom5.set_value(100);
    std::cout << "   First set_value() succeeded" << std::endl;
    
    try {
        prom5.set_value(200);  // ERROR!
    } catch (const std::future_error& e) {
        std::cout << "   Second set_value() failed: " << e.what() << std::endl;
    }
}

// ============================================================================
// DEMONSTRATION 4: set_exception()
// ============================================================================

void demo_set_exception() {
    std::cout << "\n=== DEMO 4: set_exception() ===" << std::endl;
    
    // 1. Basic exception setting
    std::cout << "\n1. Setting an exception:" << std::endl;
    std::promise<int> prom1;
    std::future<int> fut1 = prom1.get_future();
    
    std::thread worker1([](std::promise<int> p) {
        try {
            std::cout << "   Worker performing operation..." << std::endl;
            std::this_thread::sleep_for(500ms);
            
            // Simulate error condition
            throw std::runtime_error("Operation failed!");
            
            p.set_value(42);  // Never reached
        } catch (...) {
            // Capture current exception and set it in promise
            p.set_exception(std::current_exception());
            std::cout << "   Exception set in promise" << std::endl;
        }
    }, std::move(prom1));
    
    try {
        int result = fut1.get();  // Exception re-thrown here
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "   Caught exception: " << e.what() << std::endl;
    }
    
    worker1.join();
    
    // 2. Setting exception manually
    std::cout << "\n2. Manually creating and setting exception:" << std::endl;
    std::promise<double> prom2;
    std::future<double> fut2 = prom2.get_future();
    
    std::thread worker2([](std::promise<double> p) {
        std::this_thread::sleep_for(500ms);
        
        // Create exception explicitly
        try {
            throw std::invalid_argument("Invalid input provided");
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    }, std::move(prom2));
    
    try {
        double result = fut2.get();
    } catch (const std::invalid_argument& e) {
        std::cout << "   Caught: " << e.what() << std::endl;
    }
    
    worker2.join();
    
    // 3. Using make_exception_ptr
    std::cout << "\n3. Using std::make_exception_ptr:" << std::endl;
    std::promise<int> prom3;
    std::future<int> fut3 = prom3.get_future();
    
    std::thread worker3([](std::promise<int> p) {
        std::this_thread::sleep_for(500ms);
        // Create exception without throwing
        auto eptr = std::make_exception_ptr(std::logic_error("Logic error occurred"));
        p.set_exception(eptr);
    }, std::move(prom3));
    
    try {
        int result = fut3.get();
    } catch (const std::logic_error& e) {
        std::cout << "   Caught: " << e.what() << std::endl;
    }
    
    worker3.join();
}

// ============================================================================
// DEMONSTRATION 5: set_value_at_thread_exit()
// ============================================================================

void demo_set_value_at_thread_exit() {
    std::cout << "\n=== DEMO 5: set_value_at_thread_exit() ===" << std::endl;
    
    // Difference between set_value() and set_value_at_thread_exit()
    
    // 1. Using regular set_value()
    std::cout << "\n1. Regular set_value():" << std::endl;
    std::promise<int> prom1;
    std::future<int> fut1 = prom1.get_future();
    
    std::thread t1([](std::promise<int> p) {
        std::cout << "   [Thread] Setting value..." << std::endl;
        p.set_value(100);  // Value available immediately
        std::cout << "   [Thread] Value set, doing cleanup..." << std::endl;
        std::this_thread::sleep_for(1s);  // Simulate cleanup
        std::cout << "   [Thread] Thread exiting" << std::endl;
    }, std::move(prom1));
    
    std::cout << "   [Main] Waiting for value..." << std::endl;
    int result1 = fut1.get();  // Gets value immediately after set_value()
    std::cout << "   [Main] Got result: " << result1 << std::endl;
    std::cout << "   [Main] Worker thread still cleaning up..." << std::endl;
    t1.join();
    std::cout << "   [Main] Thread joined" << std::endl;
    
    // 2. Using set_value_at_thread_exit()
    std::cout << "\n2. set_value_at_thread_exit():" << std::endl;
    std::promise<int> prom2;
    std::future<int> fut2 = prom2.get_future();
    
    std::thread t2([](std::promise<int> p) {
        std::cout << "   [Thread] Setting value at exit..." << std::endl;
        p.set_value_at_thread_exit(200);  // Value available only when thread exits
        std::cout << "   [Thread] Value scheduled, doing cleanup..." << std::endl;
        std::this_thread::sleep_for(1s);  // Simulate cleanup
        std::cout << "   [Thread] Thread exiting now" << std::endl;
    }, std::move(prom2));
    
    std::cout << "   [Main] Waiting for value..." << std::endl;
    int result2 = fut2.get();  // Blocks until thread exits completely
    std::cout << "   [Main] Got result: " << result2 << std::endl;
    std::cout << "   [Main] Thread has exited" << std::endl;
    t2.join();
    
    // Use case: Ensures all thread-local cleanup is done before value is available
}

// ============================================================================
// DEMONSTRATION 6: set_exception_at_thread_exit()
// ============================================================================

void demo_set_exception_at_thread_exit() {
    std::cout << "\n=== DEMO 6: set_exception_at_thread_exit() ===" << std::endl;
    
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    
    std::thread worker([](std::promise<int> p) {
        std::cout << "   [Thread] Setting exception at exit..." << std::endl;
        
        auto eptr = std::make_exception_ptr(
            std::runtime_error("Error, but cleanup needed first")
        );
        p.set_exception_at_thread_exit(eptr);
        
        std::cout << "   [Thread] Exception scheduled, doing cleanup..." << std::endl;
        std::this_thread::sleep_for(1s);
        std::cout << "   [Thread] Cleanup done, thread exiting" << std::endl;
    }, std::move(prom));
    
    try {
        std::cout << "   [Main] Waiting for result..." << std::endl;
        int result = fut.get();  // Blocks until thread exits, then throws
    } catch (const std::runtime_error& e) {
        std::cout << "   [Main] Caught exception: " << e.what() << std::endl;
        std::cout << "   [Main] Thread cleanup was completed before exception" << std::endl;
    }
    
    worker.join();
}

// ============================================================================
// DEMONSTRATION 7: Real-World Producer-Consumer Pattern
// ============================================================================

void demo_producer_consumer() {
    std::cout << "\n=== DEMO 7: Producer-Consumer Pattern ===" << std::endl;
    
    // Producer thread generates data
    auto producer = [](std::promise<std::vector<int>> p) {
        std::cout << "   [Producer] Starting data generation..." << std::endl;
        std::vector<int> data;
        
        for (int i = 1; i <= 5; ++i) {
            std::this_thread::sleep_for(200ms);
            data.push_back(i * 10);
            std::cout << "   [Producer] Generated: " << (i * 10) << std::endl;
        }
        
        std::cout << "   [Producer] All data generated, sending to consumer" << std::endl;
        p.set_value(std::move(data));
    };
    
    // Consumer thread processes data
    auto consumer = [](std::future<std::vector<int>> f) {
        std::cout << "   [Consumer] Waiting for data..." << std::endl;
        std::vector<int> data = f.get();
        
        std::cout << "   [Consumer] Received data, processing..." << std::endl;
        int sum = 0;
        for (int val : data) {
            sum += val;
            std::cout << "   [Consumer] Processing: " << val << std::endl;
            std::this_thread::sleep_for(100ms);
        }
        
        std::cout << "   [Consumer] Total sum: " << sum << std::endl;
    };
    
    std::promise<std::vector<int>> prom;
    std::future<std::vector<int>> fut = prom.get_future();
    
    std::thread t1(producer, std::move(prom));
    std::thread t2(consumer, std::move(fut));
    
    t1.join();
    t2.join();
}

// ============================================================================
// DEMONSTRATION 8: Multiple Promises for Synchronization
// ============================================================================

void demo_multiple_promises() {
    std::cout << "\n=== DEMO 8: Multiple Promises for Synchronization ===" << std::endl;
    
    std::promise<void> start_signal;
    std::shared_future<void> ready_future = start_signal.get_future().share();
    
    // Create multiple workers waiting for start signal
    std::vector<std::thread> workers;
    
    for (int i = 1; i <= 3; ++i) {
        workers.emplace_back([i, ready_future]() {
            std::cout << "   [Worker " << i << "] Ready and waiting..." << std::endl;
            ready_future.get();  // Wait for start signal
            std::cout << "   [Worker " << i << "] GO! Starting work..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 300));
            std::cout << "   [Worker " << i << "] Work completed!" << std::endl;
        });
    }
    
    std::cout << "[Main] All workers created, preparing..." << std::endl;
    std::this_thread::sleep_for(1s);
    
    std::cout << "[Main] Sending start signal..." << std::endl;
    start_signal.set_value();  // Release all workers simultaneously
    
    for (auto& t : workers) {
        t.join();
    }
    
    std::cout << "[Main] All workers completed" << std::endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         STD::PROMISE COMPLETE GUIDE                      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_basic_promise();
    demo_constructor_get_future();
    demo_set_value();
    demo_set_exception();
    demo_set_value_at_thread_exit();
    demo_set_exception_at_thread_exit();
    demo_producer_consumer();
    demo_multiple_promises();
    
    std::cout << "\n✓ All std::promise demonstrations completed!" << std::endl;
    return 0;
}
