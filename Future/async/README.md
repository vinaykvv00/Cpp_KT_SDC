# Complete Guide: std::future, std::async, and std::promise

## 📚 Overview

This comprehensive guide covers C++11's concurrency features for asynchronous programming:
- **`std::future<T>`** - Receiving results from asynchronous operations
- **`std::async`** - Running functions asynchronously
- **`std::promise<T>`** - Manually setting values for futures

## 🎯 What Are These?

### `std::future<T>`
- **Type**: Class template
- **Header**: `<future>`
- **Purpose**: Provides mechanism to access results of asynchronous operations
- **Specializations**:
  - `std::future<T>` - for value types
  - `std::future<T&>` - for reference types
  - `std::future<void>` - for operations with no return value

### `std::async`
- **Type**: Function template
- **Header**: `<future>`
- **Purpose**: Runs functions asynchronously and returns `std::future`
- **Signature**:
  ```cpp
  template<class Function, class... Args>
  std::future<return_type> async(std::launch policy, Function&& f, Args&&... args);
  ```

### `std::promise<T>`
- **Type**: Class template
- **Header**: `<future>`
- **Purpose**: Provides a way to set a value or exception that can be retrieved via `std::future`
- **Use case**: Custom asynchronous operations with manual control

## 📂 File Structure

```
Future/async/
├── 01_future_basics.cpp              # Introduction to std::future
├── 02_future_member_functions.cpp    # All std::future member functions
├── 03_async_complete.cpp             # Complete std::async guide
├── 04_promise_complete.cpp           # Complete std::promise guide
├── CMakeLists.txt                    # Build configuration
└── README.md                         # This file
```

## 🔧 Building the Examples

### Prerequisites
- C++17 or later
- CMake 3.14 or later
- Compiler with thread support (GCC, Clang, MSVC)

### Build Commands

```bash
# Create build directory
mkdir -p build && cd build

# Configure
cmake ..

# Build all examples
cmake --build .

# Or build specific example
cmake --build . --target 01_future_basics
```

### Running Examples

```bash
# Run individual examples
./bin/01_future_basics
./bin/02_future_member_functions
./bin/03_async_complete
./bin/04_promise_complete

# Or run all examples at once
make run_all
```

## 📖 Detailed Contents

### 1. `01_future_basics.cpp`

**Concepts Covered:**
- Creating futures with `std::async`
- `std::future<void>` specialization
- `std::future<T&>` reference specialization
- Different callable types (lambdas, functors, member functions)
- Move semantics (futures are move-only)

**Key Demonstrations:**
```cpp
// Basic usage
std::future<int> fut = std::async(std::launch::async, compute, 10);
int result = fut.get();

// Void specialization
std::future<void> fut = std::async(task);
fut.get();  // Waits for completion

// Reference specialization
std::future<int&> fut = std::async(getReference);
int& ref = fut.get();
```

**Runtime:** ~5-6 seconds

---

### 2. `02_future_member_functions.cpp`

**All Member Functions Demonstrated:**

#### Constructors & Assignment
```cpp
std::future<int> fut1;              // Default constructor (invalid)
std::future<int> fut2 = std::move(fut1);  // Move constructor
fut2 = std::move(fut1);             // Move assignment
```

#### `get()` - Retrieve Result
```cpp
T get();                            // Blocks until ready, can only call ONCE
// After get(), future becomes invalid
```

#### `valid()` - Check State
```cpp
bool valid() const noexcept;        // Returns true if future has shared state
```

#### `wait()` - Wait Without Retrieving
```cpp
void wait() const;                  // Blocks until ready, doesn't consume future
```

#### `wait_for()` - Timed Wait
```cpp
std::future_status wait_for(duration);
// Returns: ready, timeout, or deferred
```

#### `wait_until()` - Wait Until Time Point
```cpp
std::future_status wait_until(time_point);
```

#### `share()` - Convert to shared_future
```cpp
std::shared_future<T> share();      // Allows multiple get() calls
```

**Runtime:** ~15-20 seconds (includes various wait operations)

---

### 3. `03_async_complete.cpp`

**Launch Policies:**

#### `std::launch::async`
```cpp
auto fut = std::async(std::launch::async, func, args...);
// - Guaranteed NEW THREAD
// - Function starts IMMEDIATELY
```

#### `std::launch::deferred`
```cpp
auto fut = std::async(std::launch::deferred, func, args...);
// - NO new thread
// - Runs on calling thread when get()/wait() called
// - LAZY EVALUATION
```

#### Default (both policies)
```cpp
auto fut = std::async(func, args...);
// Implementation chooses
```

**Parameters Covered:**
- Regular functions
- Lambda expressions
- Function objects (functors)
- Member functions (non-static)
- Static member functions
- Functions with reference parameters (`std::ref`)

**Advanced Topics:**
- Exception handling across threads
- Different return types (void, references, complex objects)
- Real-world use cases (parallel data fetching)
- First-completed wins pattern

**Runtime:** ~12-15 seconds

---

### 4. `04_promise_complete.cpp`

**Member Functions Demonstrated:**

#### Basic Operations
```cpp
std::promise<int> prom;             // Create promise
std::future<int> fut = prom.get_future();  // Get associated future (once only)
prom.set_value(42);                 // Set result (once only)
```

#### Exception Handling
```cpp
try {
    throw std::runtime_error("Error!");
} catch (...) {
    prom.set_exception(std::current_exception());
}

// Or create exception directly
auto eptr = std::make_exception_ptr(std::logic_error("Error"));
prom.set_exception(eptr);
```

#### Thread-Exit Functions
```cpp
prom.set_value_at_thread_exit(value);       // Value available when thread exits
prom.set_exception_at_thread_exit(eptr);    // Exception available when thread exits
```

**Patterns Demonstrated:**
- Basic producer-consumer
- Exception propagation across threads
- Synchronization using `promise<void>`
- Multiple consumers with `shared_future`
- Simultaneous worker release

**Runtime:** ~15-18 seconds

---

## 🎓 Comparison Table

| Feature | `std::future` | `std::async` | `std::promise` |
|---------|---------------|--------------|----------------|
| **Type** | Class template | Function template | Class template |
| **Purpose** | Receive result | Execute function async | Set result manually |
| **Creates threads?** | No | Yes (with async policy) | No |
| **Level** | High/Low | High | Low |
| **Control** | Consumer side | Automatic | Producer side |
| **Copyable** | No (move-only) | N/A | No (move-only) |
| **Use when** | Getting results | Easy parallelism | Custom async ops |

## 💡 When to Use What?

### Use `std::async` when:
- ✅ You want simple parallelism
- ✅ You have a function to run asynchronously
- ✅ You want automatic thread management
- ✅ You need the result later

### Use `std::promise` when:
- ✅ You need manual control over when values are set
- ✅ Building custom asynchronous patterns
- ✅ Implementing producer-consumer patterns
- ✅ Synchronizing multiple threads
- ✅ Setting values from different code paths

### Use `std::future` for:
- ✅ Receiving results from `async` or `promise`
- ✅ Checking if result is ready (`wait_for`)
- ✅ Waiting for asynchronous operations
- ✅ Exception propagation across threads

## 🔍 Key Concepts

### Move Semantics
```cpp
std::future<int> fut1 = std::async(func);
std::future<int> fut2 = std::move(fut1);  // OK
// std::future<int> fut3 = fut1;          // ERROR: not copyable
```

### One-Time Get
```cpp
std::future<int> fut = std::async(func);
int val = fut.get();      // OK - first call
// int val2 = fut.get();  // ERROR: can only call once
```

### Shared Futures
```cpp
std::shared_future<int> shared = fut.share();
int val1 = shared.get();  // OK
int val2 = shared.get();  // OK - can call multiple times
```

### Exception Safety
```cpp
// Exceptions are transported across threads
auto fut = std::async([]() { throw std::runtime_error("Error!"); });
try {
    fut.get();  // Exception re-thrown here
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}
```

## 🚀 Advanced Patterns

### Parallel Data Processing
```cpp
std::vector<std::future<int>> futures;
for (int i = 0; i < 10; ++i) {
    futures.push_back(std::async(std::launch::async, process, data[i]));
}
for (auto& fut : futures) {
    int result = fut.get();
    // Process result
}
```

### Synchronization Barrier
```cpp
std::promise<void> barrier;
std::shared_future<void> ready = barrier.get_future().share();

// Multiple threads wait for signal
std::thread t1([ready]() { ready.get(); /* work */ });
std::thread t2([ready]() { ready.get(); /* work */ });

barrier.set_value();  // Release all threads
```

### Timeout Handling
```cpp
auto fut = std::async(longRunningTask);
if (fut.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
    std::cout << "Task timed out!" << std::endl;
} else {
    int result = fut.get();
}
```

## 📝 Important Notes

1. **Thread Safety**: `std::future` and `std::promise` are thread-safe for their intended use
2. **Blocking**: `get()` and `wait()` block the calling thread
3. **Destruction**: If a `std::future` from `std::async` is destroyed, it blocks until complete
4. **Valid State**: Always check `valid()` before calling `get()` or `wait()`
5. **Move-Only**: Futures and promises cannot be copied, only moved
6. **One-Shot**: Values can only be set and retrieved once (unless using `shared_future`)

## 🔗 Related Topics

- `std::packaged_task` - Wraps callable and creates future
- `std::shared_future` - Multiple threads can access same result
- `std::condition_variable` - Lower-level synchronization
- `std::thread` - Direct thread management
- `std::mutex` - Mutual exclusion

## 📚 References

- [C++ Reference - std::future](https://en.cppreference.com/w/cpp/thread/future)
- [C++ Reference - std::async](https://en.cppreference.com/w/cpp/thread/async)
- [C++ Reference - std::promise](https://en.cppreference.com/w/cpp/thread/promise)

---

## ✅ Quick Start

```bash
# Clone and build
cd /home/z00542kh/cpp_KT/C++_KT/Future/async
mkdir -p build && cd build
cmake ..
cmake --build .

# Run all examples
./bin/01_future_basics
./bin/02_future_member_functions
./bin/03_async_complete
./bin/04_promise_complete
```

**Estimated total runtime:** ~45-50 seconds for all examples

---

**Created:** November 28, 2025  
**C++ Standard:** C++17  
**Author:** Comprehensive C++ Concurrency Guide
