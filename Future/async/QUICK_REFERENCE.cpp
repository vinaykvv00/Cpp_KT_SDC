// /*
//  * QUICK REFERENCE CARD: std::future, std::async, std::promise
//  * ============================================================
//  */

// #include <future>  // Required header for all three

// // ============================================================================
// // STD::FUTURE<T> - Result Receiver
// // ============================================================================
// // Type: CLASS TEMPLATE
// // Purpose: Access result of asynchronous operations
// // Copyable: NO (move-only)
// // Reusable: NO (get() can only be called once)

// std::future<int> fut;

// // MEMBER FUNCTIONS:
// fut.get()               // Get result (blocks, one-time only)
// fut.valid()             // Check if future has shared state
// fut.wait()              // Wait without retrieving (reusable)
// fut.wait_for(duration)  // Wait with timeout (returns future_status)
// fut.wait_until(time)    // Wait until time point
// fut.share()             // Convert to shared_future (multiple get() allowed)

// // SPECIALIZATIONS:
// std::future<T>          // For value types
// std::future<T&>         // For reference types
// std::future<void>       // For operations with no return

// // EXAMPLE:
// std::future<int> fut = std::async([]{ return 42; });
// int result = fut.get();  // Blocks until ready, invalidates future

// // ============================================================================
// // STD::ASYNC - Function Runner
// // ============================================================================
// // Type: FUNCTION TEMPLATE
// // Purpose: Run function asynchronously, returns std::future
// // Thread Management: Automatic

// // SIGNATURES:
// template<class F, class... Args>
// std::future<return_type> async(F&& f, Args&&... args);

// template<class F, class... Args>
// std::future<return_type> async(std::launch policy, F&& f, Args&&... args);

// // LAUNCH POLICIES:
// std::launch::async      // Guaranteed new thread, starts immediately
// std::launch::deferred   // No thread, runs on get()/wait() (lazy)
// std::launch::async | std::launch::deferred  // Implementation decides (default)

// // PARAMETERS:
// // - Can pass any callable: function, lambda, functor, member function
// // - Use std::ref() for reference parameters

// // EXAMPLES:

// // 1. Simple function
// auto fut = std::async(std::launch::async, compute, 5);

// // 2. Lambda
// auto fut = std::async([]() { return 42; });

// // 3. With arguments
// auto fut = std::async(add, 10, 20);

// // 4. Member function
// Calculator calc;
// auto fut = std::async(&Calculator::compute, &calc, 5);

// // 5. Reference parameter
// int value = 100;
// auto fut = std::async(modify, std::ref(value));

// // 6. Exception handling
// auto fut = std::async([]() { throw std::runtime_error("Error!"); });
// try {
//     fut.get();  // Exception re-thrown here
// } catch (...) { }

// // ============================================================================
// // STD::PROMISE<T> - Value Setter
// // ============================================================================
// // Type: CLASS TEMPLATE
// // Purpose: Manually set value/exception for std::future
// // Copyable: NO (move-only)
// // Reusable: NO (set_value/set_exception once only)

// std::promise<int> prom;

// // MEMBER FUNCTIONS:
// prom.get_future()       // Get associated future (once only)
// prom.set_value(val)     // Set result value (once only)
// prom.set_exception(e)   // Set exception (once only)
// prom.set_value_at_thread_exit(val)      // Set when thread exits
// prom.set_exception_at_thread_exit(e)    // Set exception when thread exits

// // SPECIALIZATIONS:
// std::promise<T>         // For value types
// std::promise<T&>        // For reference types
// std::promise<void>      // For signals with no value

// // BASIC USAGE:
// std::promise<int> prom;
// std::future<int> fut = prom.get_future();

// std::thread t([](std::promise<int> p) {
//     // Do work...
//     p.set_value(42);
// }, std::move(prom));

// int result = fut.get();  // Blocks until set_value called
// t.join();

// // EXCEPTION SETTING:
// try {
//     throw std::runtime_error("Error!");
// } catch (...) {
//     prom.set_exception(std::current_exception());
// }

// // Or create exception directly:
// auto eptr = std::make_exception_ptr(std::logic_error("Error"));
// prom.set_exception(eptr);

// // VOID SPECIALIZATION (signaling):
// std::promise<void> ready;
// std::future<void> fut = ready.get_future();
// // ... later ...
// ready.set_value();  // Signal ready
// fut.get();          // Wait for signal

// // ============================================================================
// // STD::SHARED_FUTURE<T> - Multiple Readers
// // ============================================================================
// // Like std::future but COPYABLE - multiple threads can call get()

// std::future<int> fut = std::async([]{ return 42; });
// std::shared_future<int> shared_fut = fut.share();

// // Now multiple threads can access:
// std::thread t1([shared_fut]() { int val = shared_fut.get(); });
// std::thread t2([shared_fut]() { int val = shared_fut.get(); });

// // ============================================================================
// // FUTURE_STATUS - Wait Result
// // ============================================================================
// enum class future_status {
//     ready,      // Result is ready
//     timeout,    // Timeout expired
//     deferred    // Function not started yet (launch::deferred)
// };

// // Usage:
// if (fut.wait_for(1s) == std::future_status::ready) {
//     int result = fut.get();
// }

// // ============================================================================
// // COMMON PATTERNS
// // ============================================================================

// // PATTERN 1: Parallel Execution
// std::vector<std::future<int>> futures;
// for (auto& item : items) {
//     futures.push_back(std::async(std::launch::async, process, item));
// }
// for (auto& fut : futures) {
//     results.push_back(fut.get());
// }

// // PATTERN 2: Producer-Consumer
// std::promise<Data> prom;
// std::future<Data> fut = prom.get_future();

// std::thread producer([](std::promise<Data> p) {
//     Data data = generate();
//     p.set_value(std::move(data));
// }, std::move(prom));

// Data result = fut.get();  // Consumer waits
// producer.join();

// // PATTERN 3: Timeout Handling
// auto fut = std::async(long_task);
// if (fut.wait_for(5s) == std::future_status::timeout) {
//     std::cout << "Timeout!" << std::endl;
// } else {
//     auto result = fut.get();
// }

// // PATTERN 4: Synchronization Barrier
// std::promise<void> barrier;
// std::shared_future<void> ready = barrier.get_future().share();

// std::thread t1([ready]() { ready.get(); /* work */ });
// std::thread t2([ready]() { ready.get(); /* work */ });
// std::thread t3([ready]() { ready.get(); /* work */ });

// barrier.set_value();  // Release all threads

// // ============================================================================
// // KEY DIFFERENCES
// // ============================================================================

// /*
// ┌─────────────┬────────────────┬─────────────────┬─────────────────┐
// │ Feature     │ std::future    │ std::async      │ std::promise    │
// ├─────────────┼────────────────┼─────────────────┼─────────────────┤
// │ Type        │ Class template │ Function        │ Class template  │
// │ Purpose     │ Receive result │ Run async       │ Set result      │
// │ Thread?     │ No             │ Yes (optional)  │ No              │
// │ Control     │ Consumer       │ Automatic       │ Producer        │
// │ Copyable    │ No             │ N/A             │ No              │
// │ Level       │ High/Low       │ High            │ Low             │
// └─────────────┴────────────────┴─────────────────┴─────────────────┘
// */

// // ============================================================================
// // WHEN TO USE WHAT?
// // ============================================================================

// // Use std::async when:
// // ✓ Simple parallelism needed
// // ✓ Want automatic thread management
// // ✓ Have a function to run asynchronously

// // Use std::promise when:
// // ✓ Need manual control over value setting
// // ✓ Building custom async patterns
// // ✓ Value comes from different code paths

// // Use std::future for:
// // ✓ Receiving results from async/promise
// // ✓ Checking if result is ready
// // ✓ Waiting for async operations

// // ============================================================================
// // IMPORTANT NOTES
// // ============================================================================

// /*
// 1. Move-Only: futures and promises cannot be copied, only moved
// 2. One-Time Use: get() and set_value() can only be called once
// 3. Blocking: get() and wait() block the calling thread
// 4. Thread-Safe: For their intended use (setting/getting values)
// 5. Exception Transport: Exceptions cross thread boundaries
// 6. Destruction: future from async blocks on destruction if not retrieved
// 7. Valid State: Check valid() before calling get() or wait()
// 8. Shared Future: Use share() for multiple consumers
// */

// // ============================================================================
// // COMPILE & LINK
// // ============================================================================

// /*
// Compiler flags:
//   g++ -std=c++17 -pthread program.cpp -o program
//   clang++ -std=c++17 -pthread program.cpp -o program

// CMake:
//   find_package(Threads REQUIRED)
//   target_link_libraries(target PRIVATE Threads::Threads)

// Header:
//   #include <future>
// */
