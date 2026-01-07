/**
 * @file 03_async_complete.cpp
 * @brief Complete demonstration of std::async
 * 
 * WHAT IS std::async?
 * - A FUNCTION TEMPLATE (not a class or keyword)
 * - Defined in <future> header
 * - Runs a function asynchronously and returns std::future
 * - Manages thread creation automatically
 * 
 * FUNCTION SIGNATURES:
 * template< class Function, class... Args >
 * std::future<return_type> async(std::launch policy, Function&& f, Args&&... args);
 * 
 * template< class Function, class... Args >
 * std::future<return_type> async(Function&& f, Args&&... args);
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

using namespace std::chrono_literals;

// ============================================================================
// DEMONSTRATION 1: std::launch Policies
// ============================================================================

int computation(const std::string& name, int value) {
    std::cout << "  [" << name << " - Thread " << std::this_thread::get_id() 
              << "] Computing..." << std::endl;
    std::this_thread::sleep_for(1s);
    return value * value;
}

void demo_launch_policies() {
    std::cout << "\n=== DEMO 1: std::launch Policies ===" << std::endl;
    std::cout << "[Main thread: " << std::this_thread::get_id() << "]" << std::endl;
    
    // POLICY 1: std::launch::async
    // - GUARANTEES execution in a NEW THREAD
    // - Function starts IMMEDIATELY
    std::cout << "\n1. std::launch::async (guaranteed new thread):" << std::endl;
    auto fut1 = std::async(std::launch::async, computation, "ASYNC", 5);
    std::cout << "   async() returned immediately, task running in background" << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "   Main thread doing other work..." << std::endl;
    std::cout << "   Result: " << fut1.get() << std::endl;
    
    // POLICY 2: std::launch::deferred
    // - NO NEW THREAD created
    // - Function runs on CALLING THREAD when get() or wait() is called
    // - LAZY EVALUATION
    std::cout << "\n2. std::launch::deferred (lazy evaluation):" << std::endl;
    auto fut2 = std::async(std::launch::deferred, computation, "DEFERRED", 6);
    std::cout << "   async() returned, but function NOT started yet" << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "   Main thread still doing work..." << std::endl;
    std::cout << "   Now calling get() - function will run NOW on this thread:" << std::endl;
    std::cout << "   Result: " << fut2.get() << std::endl;
    
    // POLICY 3: std::launch::async | std::launch::deferred (DEFAULT)
    // - Implementation chooses
    // - May or may not create a thread
    std::cout << "\n3. Default policy (async | deferred):" << std::endl;
    auto fut3 = std::async(computation, "DEFAULT", 7);
    std::cout << "   Implementation decides whether to create thread or defer" << std::endl;
    std::cout << "   Result: " << fut3.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 2: std::async Parameters - All Callable Types
// ============================================================================

// Regular function
int add(int a, int b) {
    std::cout << "  add(" << a << ", " << b << ") = " << (a + b) << std::endl;
    return a + b;
}

// Function with reference parameter
void modifyValue(int& value) {
    std::cout << "  Modifying value from " << value;
    value *= 2;
    std::cout << " to " << value << std::endl;
}

// Function object (functor)
struct Multiplier {
    int factor;
    Multiplier(int f) : factor(f) {}
    
    int operator()(int x) const {
        std::cout << "  Functor: " << x << " * " << factor << " = " << (x * factor) << std::endl;
        return x * factor;
    }
};

// Class with member functions
class MathOperations {
public:
    int value;
    
    MathOperations(int v) : value(v) {}
    
    int square() const {
        std::cout << "  Member function: " << value << "^2 = " << (value * value) << std::endl;
        return value * value;
    }
    
    int add(int x) const {
        std::cout << "  Member function: " << value << " + " << x << " = " << (value + x) << std::endl;
        return value + x;
    }
    
    static int multiply(int a, int b) {
        std::cout << "  Static member: " << a << " * " << b << " = " << (a * b) << std::endl;
        return a * b;
    }
};

void demo_async_parameters() {
    std::cout << "\n=== DEMO 2: std::async Parameters ===" << std::endl;
    
    // 1. Regular function with arguments
    std::cout << "\n1. Regular function:" << std::endl;
    auto fut1 = std::async(std::launch::async, add, 10, 20);
    std::cout << "   Result: " << fut1.get() << std::endl;
    
    // 2. Function with reference (use std::ref)
    std::cout << "\n2. Function with reference parameter:" << std::endl;
    int value = 50;
    std::cout << "   Before: " << value << std::endl;
    auto fut2 = std::async(std::launch::async, modifyValue, std::ref(value));
    fut2.wait();
    std::cout << "   After: " << value << std::endl;
    
    // 3. Lambda function
    std::cout << "\n3. Lambda function:" << std::endl;
    auto fut3 = std::async(std::launch::async, [](int a, int b, int c) {
        int result = a + b + c;
        std::cout << "  Lambda: " << a << " + " << b << " + " << c << " = " << result << std::endl;
        return result;
    }, 1, 2, 3);
    std::cout << "   Result: " << fut3.get() << std::endl;
    
    // 4. Function object (functor)
    std::cout << "\n4. Function object (functor):" << std::endl;
    Multiplier mult(5);
    auto fut4 = std::async(std::launch::async, mult, 7);
    std::cout << "   Result: " << fut4.get() << std::endl;
    
    // 5. Member function (non-static)
    std::cout << "\n5. Member function:" << std::endl;
    MathOperations math(10);
    auto fut5 = std::async(std::launch::async, &MathOperations::square, &math);
    std::cout << "   Result: " << fut5.get() << std::endl;
    
    // 6. Member function with parameters
    std::cout << "\n6. Member function with parameters:" << std::endl;
    auto fut6 = std::async(std::launch::async, &MathOperations::add, &math, 15);
    std::cout << "   Result: " << fut6.get() << std::endl;
    
    // 7. Static member function
    std::cout << "\n7. Static member function:" << std::endl;
    auto fut7 = std::async(std::launch::async, &MathOperations::multiply, 4, 5);
    std::cout << "   Result: " << fut7.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 3: Exception Handling with std::async
// ============================================================================

int riskyOperation(int value) {
    std::cout << "  Performing risky operation with value: " << value << std::endl;
    std::this_thread::sleep_for(500ms);
    
    if (value < 0) {
        throw std::invalid_argument("Negative values not allowed!");
    }
    if (value > 100) {
        throw std::out_of_range("Value too large!");
    }
    
    return value * 2;
}

void demo_exception_handling() {
    std::cout << "\n=== DEMO 3: Exception Handling ===" << std::endl;
    
    // Test 1: Successful execution
    std::cout << "\n1. Successful execution:" << std::endl;
    auto fut1 = std::async(std::launch::async, riskyOperation, 50);
    try {
        int result = fut1.get();
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   Exception: " << e.what() << std::endl;
    }
    
    // Test 2: Exception - negative value
    std::cout << "\n2. Exception handling - negative value:" << std::endl;
    auto fut2 = std::async(std::launch::async, riskyOperation, -5);
    try {
        int result = fut2.get();  // Exception thrown here
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "   Caught exception: " << e.what() << std::endl;
    }
    
    // Test 3: Exception - value too large
    std::cout << "\n3. Exception handling - out of range:" << std::endl;
    auto fut3 = std::async(std::launch::async, riskyOperation, 150);
    try {
        int result = fut3.get();
        std::cout << "   Result: " << result << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "   Caught exception: " << e.what() << std::endl;
    }
    
    // Test 4: Multiple operations with exceptions
    std::cout << "\n4. Multiple async operations:" << std::endl;
    std::vector<std::future<int>> futures;
    std::vector<int> values = {10, -5, 50, 200, 30};
    
    for (int val : values) {
        futures.push_back(std::async(std::launch::async, riskyOperation, val));
    }
    
    for (size_t i = 0; i < futures.size(); ++i) {
        try {
            int result = futures[i].get();
            std::cout << "   Value " << values[i] << " -> Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "   Value " << values[i] << " -> Exception: " << e.what() << std::endl;
        }
    }
}

// ============================================================================
// DEMONSTRATION 4: Return Types
// ============================================================================

// Function returning void
void logMessage(const std::string& msg) {
    std::cout << "  [LOG] " << msg << std::endl;
    std::this_thread::sleep_for(300ms);
}

// Function returning reference
std::string globalData = "Initial Data";

std::string& getDataReference() {
    std::cout << "  Returning reference to global data" << std::endl;
    std::this_thread::sleep_for(200ms);
    return globalData;
}

// Function returning complex object
struct Result {
    int code;
    std::string message;
    
    Result(int c, std::string m) : code(c), message(std::move(m)) {
        std::cout << "  Result object created: [" << code << "] " << message << std::endl;
    }
};

Result createResult() {
    std::this_thread::sleep_for(400ms);
    return Result(200, "Operation Successful");
}

void demo_return_types() {
    std::cout << "\n=== DEMO 4: Different Return Types ===" << std::endl;
    
    // 1. Void return type
    std::cout << "\n1. void return type (std::future<void>):" << std::endl;
    auto fut1 = std::async(std::launch::async, logMessage, "Async void operation");
    fut1.get();  // Returns void, just waits for completion
    std::cout << "   Void task completed" << std::endl;
    
    // 2. Reference return type
    std::cout << "\n2. Reference return type (std::future<std::string&>):" << std::endl;
    std::cout << "   Global data before: " << globalData << std::endl;
    auto fut2 = std::async(std::launch::async, getDataReference);
    std::string& ref = fut2.get();
    ref = "Modified Data";
    std::cout << "   Global data after modification: " << globalData << std::endl;
    
    // 3. Complex object return type
    std::cout << "\n3. Complex object return type:" << std::endl;
    auto fut3 = std::async(std::launch::async, createResult);
    Result res = fut3.get();
    std::cout << "   Retrieved result: [" << res.code << "] " << res.message << std::endl;
    
    // 4. Multiple return values using tuple
    std::cout << "\n4. Multiple return values using tuple:" << std::endl;
    auto fut4 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(300ms);
        return std::make_tuple(42, std::string("Hello"), 3.14);
    });
    auto [num, str, pi] = fut4.get();
    std::cout << "   Results: " << num << ", " << str << ", " << pi << std::endl;
}

// ============================================================================
// DEMONSTRATION 5: Real-World Use Cases
// ============================================================================

// Simulate database query
std::string queryDatabase(int userId) {
    std::cout << "  [DB] Querying user " << userId << "..." << std::endl;
    std::this_thread::sleep_for(1s);
    return "User_" + std::to_string(userId) + "_Data";
}

// Simulate API call
std::string callAPI(const std::string& endpoint) {
    std::cout << "  [API] Calling " << endpoint << "..." << std::endl;
    std::this_thread::sleep_for(800ms);
    return "Response from " + endpoint;
}

// Simulate file processing
int processFile(const std::string& filename) {
    std::cout << "  [FILE] Processing " << filename << "..." << std::endl;
    std::this_thread::sleep_for(1200ms);
    return filename.length() * 100;  // Simulated result
}

void demo_real_world_cases() {
    std::cout << "\n=== DEMO 5: Real-World Use Cases ===" << std::endl;
    
    // Use case 1: Parallel data fetching
    std::cout << "\n1. Parallel data fetching:" << std::endl;
    auto start = std::chrono::steady_clock::now();
    
    auto dbFuture = std::async(std::launch::async, queryDatabase, 12345);
    auto apiFuture = std::async(std::launch::async, callAPI, "/api/data");
    auto fileFuture = std::async(std::launch::async, processFile, "data.txt");
    
    std::string dbResult = dbFuture.get();
    std::string apiResult = apiFuture.get();
    int fileResult = fileFuture.get();
    
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n   Results:" << std::endl;
    std::cout << "   - Database: " << dbResult << std::endl;
    std::cout << "   - API: " << apiResult << std::endl;
    std::cout << "   - File: " << fileResult << " bytes" << std::endl;
    std::cout << "   Total time: " << elapsed.count() << "ms (parallel execution!)" << std::endl;
    
    // Use case 2: First-completed wins
    std::cout << "\n2. First-completed wins pattern:" << std::endl;
    std::vector<std::future<int>> tasks;
    
    for (int i = 0; i < 3; ++i) {
        tasks.push_back(std::async(std::launch::async, [i]() {
            int delay = (3 - i) * 500;  // Different delays
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            std::cout << "   Task " << i << " completed!" << std::endl;
            return i;
        }));
    }
    
    // Wait for all with timeout checking
    bool allComplete = false;
    while (!allComplete) {
        allComplete = true;
        for (auto& fut : tasks) {
            if (fut.wait_for(100ms) != std::future_status::ready) {
                allComplete = false;
            }
        }
        if (!allComplete) {
            std::cout << "   Waiting for tasks..." << std::endl;
        }
    }
    
    std::cout << "   All tasks completed!" << std::endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         STD::ASYNC COMPLETE GUIDE                        ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_launch_policies();
    demo_async_parameters();
    demo_exception_handling();
    demo_return_types();
    demo_real_world_cases();
    
    std::cout << "\n✓ All std::async demonstrations completed!" << std::endl;
    return 0;
}
