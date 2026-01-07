/**
 * @file 01_future_basics.cpp
 * @brief Complete demonstration of std::future basics
 * 
 * WHAT IS std::future?
 * - A CLASS TEMPLATE (not a keyword or function)
 * - Defined in <future> header
 * - Provides mechanism to access result of asynchronous operations
 * - Three specializations:
 *   1. std::future<T>      - for value types
 *   2. std::future<T&>     - for reference types
 *   3. std::future<void>   - for operations with no return value
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

// ============================================================================
// DEMONSTRATION 1: Creating futures with std::async
// ============================================================================

int expensiveComputation(int x) {
    std::cout << "  [Thread " << std::this_thread::get_id() 
              << "] Computing " << x << "^2..." << std::endl;
    std::this_thread::sleep_for(2s);
    return x * x;
}

void demo_future_creation() {
    std::cout << "\n=== DEMO 1: Creating std::future ===" << std::endl;
    std::cout << "[Main thread: " << std::this_thread::get_id() << "]" << std::endl;
    
    // Create a future using std::async
    // std::async returns std::future<int> because expensiveComputation returns int
    std::future<int> fut = std::async(std::launch::async, expensiveComputation, 10);
    
    std::cout << "Future created! Doing other work..." << std::endl;
    std::cout << "Main thread is free to do other things..." << std::endl;
    
    // Get the result (blocks until ready)
    int result = fut.get();
    std::cout << "Result received: " << result << std::endl;
}

// ============================================================================
// DEMONSTRATION 2: std::future<void> specialization
// ============================================================================

void taskWithNoReturn() {
    std::cout << "  [Thread " << std::this_thread::get_id() 
              << "] Performing task with no return value..." << std::endl;
    std::this_thread::sleep_for(1s);
    std::cout << "  Task completed!" << std::endl;
}

void demo_future_void() {
    std::cout << "\n=== DEMO 2: std::future<void> ===" << std::endl;
    
    // future<void> specialization - for functions that return nothing
    std::future<void> fut = std::async(std::launch::async, taskWithNoReturn);
    
    std::cout << "Waiting for void task to complete..." << std::endl;
    fut.get();  // Blocks until task completes (returns void)
    std::cout << "Void task confirmed complete!" << std::endl;
}

// ============================================================================
// DEMONSTRATION 3: std::future<T&> reference specialization
// ============================================================================

int globalValue = 100;

int& getReference() {
    std::cout << "  Returning reference to global value..." << std::endl;
    std::this_thread::sleep_for(500ms);
    return globalValue;
}

void demo_future_reference() {
    std::cout << "\n=== DEMO 3: std::future<T&> Reference ===" << std::endl;
    std::cout << "Original global value: " << globalValue << std::endl;
    
    // future<int&> specialization - for functions that return references
    std::future<int&> fut = std::async(std::launch::async, getReference);
    
    int& ref = fut.get();  // Get reference
    std::cout << "Got reference, value: " << ref << std::endl;
    
    ref = 999;  // Modify through reference
    std::cout << "Modified through reference" << std::endl;
    std::cout << "Global value now: " << globalValue << std::endl;
}

// ============================================================================
// DEMONSTRATION 4: Future with different callable types
// ============================================================================

// Function object (functor)
struct Multiplier {
    int operator()(int a, int b) const {
        std::cout << "  Functor called: " << a << " * " << b << std::endl;
        return a * b;
    }
};

class Calculator {
public:
    int add(int a, int b) {
        std::cout << "  Member function called: " << a << " + " << b << std::endl;
        return a + b;
    }
    
    static int subtract(int a, int b) {
        std::cout << "  Static member function called: " << a << " - " << b << std::endl;
        return a - b;
    }
};

void demo_future_callables() {
    std::cout << "\n=== DEMO 4: Future with Different Callables ===" << std::endl;
    
    // 1. Lambda function
    std::future<int> fut1 = std::async(std::launch::async, [](int x) {
        std::cout << "  Lambda called: " << x << " * 2" << std::endl;
        return x * 2;
    }, 5);
    
    // 2. Function object (functor)
    Multiplier mult;
    std::future<int> fut2 = std::async(std::launch::async, mult, 3, 4);
    
    // 3. Member function
    Calculator calc;
    std::future<int> fut3 = std::async(std::launch::async, 
                                       &Calculator::add, &calc, 10, 20);
    
    // 4. Static member function
    std::future<int> fut4 = std::async(std::launch::async, 
                                       &Calculator::subtract, 50, 15);
    
    // Get all results
    std::cout << "\nResults:" << std::endl;
    std::cout << "  Lambda result: " << fut1.get() << std::endl;
    std::cout << "  Functor result: " << fut2.get() << std::endl;
    std::cout << "  Member function result: " << fut3.get() << std::endl;
    std::cout << "  Static function result: " << fut4.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 5: Understanding move semantics
// ============================================================================

void demo_future_move_semantics() {
    std::cout << "\n=== DEMO 5: Future Move Semantics ===" << std::endl;
    
    std::future<int> fut1 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(500ms);
        return 42;
    });
    
    std::cout << "fut1 valid: " << std::boolalpha << fut1.valid() << std::endl;
    
    // Move future (futures are move-only, cannot be copied)
    std::future<int> fut2 = std::move(fut1);
    
    std::cout << "After move:" << std::endl;
    std::cout << "  fut1 valid: " << fut1.valid() << std::endl;  // false
    std::cout << "  fut2 valid: " << fut2.valid() << std::endl;  // true
    
    std::cout << "  Result from fut2: " << fut2.get() << std::endl;
    std::cout << "  After get(), fut2 valid: " << fut2.valid() << std::endl;  // false
    
    // UNCOMMENT TO SEE ERROR: futures cannot be copied
    // std::future<int> fut3 = fut2;  // ERROR: future is not copyable
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         STD::FUTURE BASICS - COMPLETE GUIDE              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_future_creation();
    demo_future_void();
    demo_future_reference();
    demo_future_callables();
    demo_future_move_semantics();
    
    std::cout << "\n✓ All demonstrations completed successfully!" << std::endl;
    return 0;
}
