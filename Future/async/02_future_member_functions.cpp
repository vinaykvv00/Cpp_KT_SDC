/**
 * @file 02_future_member_functions.cpp
 * @brief Complete demonstration of ALL std::future member functions
 * 
 * MEMBER FUNCTIONS COVERED:
 * 1. Constructor (default, move)
 * 2. Destructor
 * 3. operator= (move assignment)
 * 4. get() - Get the result
 * 5. valid() - Check if future has shared state
 * 6. wait() - Wait for result without retrieving
 * 7. wait_for() - Wait with timeout
 * 8. wait_until() - Wait until specific time
 * 9. share() - Convert to shared_future
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace std::chrono_literals;

// ============================================================================
// DEMONSTRATION 1: Constructor and Destructor
// ============================================================================

void demo_constructor_destructor() {
    std::cout << "\n=== DEMO 1: Constructor & Destructor ===" << std::endl;
    
    // 1. Default constructor - creates invalid future
    std::future<int> fut1;
    std::cout << "Default constructed future valid: " << std::boolalpha 
              << fut1.valid() << std::endl;
    
    // 2. Future from async (implicit constructor)
    std::future<int> fut2 = std::async(std::launch::async, []() { 
        return 42; 
    });
    std::cout << "Future from async valid: " << fut2.valid() << std::endl;
    
    // 3. Move constructor
    std::future<int> fut3 = std::move(fut2);
    std::cout << "After move constructor:" << std::endl;
    std::cout << "  fut2 valid: " << fut2.valid() << std::endl;
    std::cout << "  fut3 valid: " << fut3.valid() << std::endl;
    
    // Destructor automatically called when going out of scope
    // It will wait for the async operation if not yet retrieved
}

// ============================================================================
// DEMONSTRATION 2: operator= (Move Assignment)
// ============================================================================

void demo_move_assignment() {
    std::cout << "\n=== DEMO 2: Move Assignment Operator ===" << std::endl;
    
    std::future<int> fut1 = std::async(std::launch::async, []() { 
        std::this_thread::sleep_for(1s);
        return 100; 
    });
    
    std::future<int> fut2 = std::async(std::launch::async, []() { 
        return 200; 
    });
    
    std::cout << "Before assignment:" << std::endl;
    std::cout << "  fut1 valid: " << fut1.valid() << std::endl;
    std::cout << "  fut2 valid: " << fut2.valid() << std::endl;
    
    // Move assignment - fut2's state is abandoned, fut1's state moved to fut2
    fut2 = std::move(fut1);
    
    std::cout << "After assignment (fut2 = move(fut1)):" << std::endl;
    std::cout << "  fut1 valid: " << fut1.valid() << std::endl;
    std::cout << "  fut2 valid: " << fut2.valid() << std::endl;
    std::cout << "  fut2 result: " << fut2.get() << std::endl;
}

// ============================================================================
// DEMONSTRATION 3: get() - Getting the Result
// ============================================================================

void demo_get() {
    std::cout << "\n=== DEMO 3: get() Member Function ===" << std::endl;
    
    // 1. Basic get()
    std::future<int> fut1 = std::async(std::launch::async, []() {
        std::cout << "  Computing result..." << std::endl;
        std::this_thread::sleep_for(1s);
        return 42;
    });
    
    std::cout << "Before get() - future valid: " << fut1.valid() << std::endl;
    int result = fut1.get();  // BLOCKS until ready
    std::cout << "Result: " << result << std::endl;
    std::cout << "After get() - future valid: " << fut1.valid() << std::endl;
    
    // 2. get() can only be called ONCE
    try {
        // This will throw because get() already called
        // int result2 = fut1.get();  // Uncommenting causes std::future_error
    } catch (const std::future_error& e) {
        std::cout << "Error calling get() twice: " << e.what() << std::endl;
    }
    
    // 3. get() with exception
    std::cout << "\nTesting exception propagation:" << std::endl;
    std::future<int> fut2 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(500ms);
        throw std::runtime_error("Something went wrong!");
        return 99;  // Never reached
    });
    
    try {
        int result2 = fut2.get();  // Exception is re-thrown here
    } catch (const std::runtime_error& e) {
        std::cout << "Caught exception from future: " << e.what() << std::endl;
    }
}

// ============================================================================
// DEMONSTRATION 4: valid() - Check Validity
// ============================================================================

void demo_valid() {
    std::cout << "\n=== DEMO 4: valid() Member Function ===" << std::endl;
    
    std::future<int> fut1;  // Default constructed
    std::cout << "1. Default constructed future valid: " << fut1.valid() << std::endl;
    
    fut1 = std::async(std::launch::async, []() { return 42; });
    std::cout << "2. After async assignment valid: " << fut1.valid() << std::endl;
    
    std::future<int> fut2 = std::move(fut1);
    std::cout << "3. After move:" << std::endl;
    std::cout << "   fut1 valid: " << fut1.valid() << std::endl;
    std::cout << "   fut2 valid: " << fut2.valid() << std::endl;
    
    int result = fut2.get();
    std::cout << "4. After get(), fut2 valid: " << fut2.valid() << std::endl;
    
    // Important: Only call get(), wait(), etc. if valid() returns true
}

// ============================================================================
// DEMONSTRATION 5: wait() - Wait Without Retrieving
// ============================================================================

void demo_wait() {
    std::cout << "\n=== DEMO 5: wait() Member Function ===" << std::endl;
    
    auto start = std::chrono::steady_clock::now();
    
    std::future<int> fut = std::async(std::launch::async, []() {
        std::cout << "  [Worker] Starting computation..." << std::endl;
        std::this_thread::sleep_for(2s);
        std::cout << "  [Worker] Computation complete!" << std::endl;
        return 42;
    });
    
    std::cout << "Calling wait()..." << std::endl;
    fut.wait();  // BLOCKS until result is ready, but doesn't retrieve it
    
    auto after_wait = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(after_wait - start);
    std::cout << "wait() returned after " << elapsed.count() << "ms" << std::endl;
    
    // Future is still valid, we can still get the result
    std::cout << "Future still valid: " << fut.valid() << std::endl;
    int result = fut.get();  // This returns immediately since result is ready
    std::cout << "Result: " << result << std::endl;
}

// ============================================================================
// DEMONSTRATION 6: wait_for() - Timed Wait
// ============================================================================

void demo_wait_for() {
    std::cout << "\n=== DEMO 6: wait_for() Member Function ===" << std::endl;
    
    // Test 1: Task completes within timeout
    std::cout << "\nTest 1: Task completes quickly" << std::endl;
    std::future<int> fut1 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(500ms);
        return 42;
    });
    
    std::future_status status1 = fut1.wait_for(1s);
    
    // wait_for returns one of three values:
    // - future_status::ready - result is ready
    // - future_status::timeout - timeout expired
    // - future_status::deferred - function hasn't started (launch::deferred)
    
    switch (status1) {
        case std::future_status::ready:
            std::cout << "  Status: READY - Result: " << fut1.get() << std::endl;
            break;
        case std::future_status::timeout:
            std::cout << "  Status: TIMEOUT" << std::endl;
            break;
        case std::future_status::deferred:
            std::cout << "  Status: DEFERRED" << std::endl;
            break;
    }
    
    // Test 2: Task times out
    std::cout << "\nTest 2: Task takes too long" << std::endl;
    std::future<int> fut2 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(3s);
        return 99;
    });
    
    std::future_status status2 = fut2.wait_for(1s);
    
    if (status2 == std::future_status::timeout) {
        std::cout << "  Task timed out! Still running in background..." << std::endl;
        std::cout << "  Waiting a bit more..." << std::endl;
        fut2.wait();  // Wait for completion
        std::cout << "  Result: " << fut2.get() << std::endl;
    }
    
    // Test 3: Deferred execution
    std::cout << "\nTest 3: Deferred execution" << std::endl;
    std::future<int> fut3 = std::async(std::launch::deferred, []() {
        std::cout << "  This runs only when get() or wait() is called!" << std::endl;
        return 777;
    });
    
    std::future_status status3 = fut3.wait_for(0s);
    
    if (status3 == std::future_status::deferred) {
        std::cout << "  Status: DEFERRED (hasn't started yet)" << std::endl;
        std::cout << "  Calling get() to execute..." << std::endl;
        std::cout << "  Result: " << fut3.get() << std::endl;
    }
}

// ============================================================================
// DEMONSTRATION 7: wait_until() - Wait Until Time Point
// ============================================================================

void demo_wait_until() {
    std::cout << "\n=== DEMO 7: wait_until() Member Function ===" << std::endl;
    
    std::future<int> fut = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(2s);
        return 42;
    });
    
    // Set deadline: 3 seconds from now
    auto deadline = std::chrono::system_clock::now() + 3s;
    
    std::cout << "Waiting until deadline (3 seconds)..." << std::endl;
    std::future_status status = fut.wait_until(deadline);
    
    if (status == std::future_status::ready) {
        std::cout << "  Task completed before deadline!" << std::endl;
        std::cout << "  Result: " << fut.get() << std::endl;
    } else if (status == std::future_status::timeout) {
        std::cout << "  Deadline reached, task not complete!" << std::endl;
    }
    
    // Example with absolute time point
    std::cout << "\nExample with specific time point:" << std::endl;
    std::future<std::string> fut2 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(1s);
        return std::string("Hello from the future!");
    });
    
    auto target_time = std::chrono::system_clock::now() + 500ms;
    if (fut2.wait_until(target_time) == std::future_status::ready) {
        std::cout << "  " << fut2.get() << std::endl;
    } else {
        std::cout << "  Not ready yet, waiting more..." << std::endl;
        std::cout << "  " << fut2.get() << std::endl;
    }
}

// ============================================================================
// DEMONSTRATION 8: share() - Convert to shared_future
// ============================================================================

void demo_share() {
    std::cout << "\n=== DEMO 8: share() Member Function ===" << std::endl;
    
    std::future<int> fut = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(1s);
        return 42;
    });
    
    std::cout << "Original future valid: " << fut.valid() << std::endl;
    
    // Convert to shared_future (allows multiple get() calls)
    std::shared_future<int> shared_fut = fut.share();
    
    std::cout << "After share():" << std::endl;
    std::cout << "  Original future valid: " << fut.valid() << std::endl;  // false
    std::cout << "  Shared future valid: " << shared_fut.valid() << std::endl;  // true
    
    // shared_future can be called multiple times and from multiple threads
    std::cout << "First get(): " << shared_fut.get() << std::endl;
    std::cout << "Second get(): " << shared_fut.get() << std::endl;  // OK with shared_future!
    std::cout << "Third get(): " << shared_fut.get() << std::endl;   // Still OK!
    
    // Use case: Multiple threads need the same result
    std::cout << "\nMultiple threads accessing shared_future:" << std::endl;
    std::shared_future<int> shared = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(500ms);
        return 100;
    }).share();
    
    std::thread t1([shared]() {
        std::cout << "  Thread 1 got: " << shared.get() << std::endl;
    });
    
    std::thread t2([shared]() {
        std::cout << "  Thread 2 got: " << shared.get() << std::endl;
    });
    
    std::thread t3([shared]() {
        std::cout << "  Thread 3 got: " << shared.get() << std::endl;
    });
    
    t1.join();
    t2.join();
    t3.join();
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║      STD::FUTURE MEMBER FUNCTIONS - COMPLETE GUIDE       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_constructor_destructor();
    demo_move_assignment();
    demo_get();
    demo_valid();
    demo_wait();
    demo_wait_for();
    demo_wait_until();
    demo_share();
    
    std::cout << "\n✓ All member function demonstrations completed!" << std::endl;
    return 0;
}
