# Complete Guide to std::packaged_task

## 📚 What is `std::packaged_task`?

### Definition
**`std::packaged_task`** is a **CLASS TEMPLATE** (not a function or keyword) that:
- Wraps any callable object (function, lambda, functor)
- Automatically creates a `std::future` for the result
- Allows **manual control** over when the function executes
- Combines the functionality of a callable + `std::promise`

### Think of it as:
A "task package" that you can:
1. ✅ Create with a function
2. ✅ Get a future for the result  
3. ✅ Execute later (you control when)
4. ✅ Pass to threads or call directly

---

## 🎯 Template Signature

```cpp
#include <future>

std::packaged_task<ReturnType(ParameterTypes...)>
                    ↑           ↑
                    |           └─ Parameter types
                    └─ Return type
```

### Examples:
```cpp
std::packaged_task<int(int)>                  // int func(int)
std::packaged_task<void()>                    // void func()
std::packaged_task<double(int, double)>       // double func(int, double)
std::packaged_task<std::string(std::string)>  // string func(string)
```

---

## 🔧 How It Works

### Basic Flow:

```cpp
// Step 1: Create packaged_task with a function
std::packaged_task<int(int)> task([](int x) { return x * x; });

// Step 2: Get the future (before execution)
std::future<int> fut = task.get_future();

// Step 3: Execute the task (YOU decide when!)
task(5);  // Function runs NOW

// Step 4: Get the result
int result = fut.get();  // 25
```

### Key Point:
**YOU control when the task executes!** Unlike `std::async` which starts immediately.

---

## 📋 All Member Functions

| Function | Description | Example |
|----------|-------------|---------|
| **Constructor** | Create from callable | `packaged_task<int(int)> task(func);` |
| **get_future()** | Get associated future (once!) | `future<int> fut = task.get_future();` |
| **operator()** | Execute the wrapped function | `task(5);  // Runs the function` |
| **valid()** | Check if task has shared state | `bool ok = task.valid();` |
| **reset()** | Reset for reuse (new future) | `task.reset();` |
| **swap()** | Swap two tasks | `task1.swap(task2);` |
| **make_ready_at_thread_exit()** | Execute, result ready at thread exit | `task.make_ready_at_thread_exit(args);` |

---

## 💡 Comparison with async and promise

### The Three Ways:

```cpp
// METHOD 1: std::async (Automatic, Immediate)
auto fut = std::async(std::launch::async, compute, 5);
// ✓ Function starts IMMEDIATELY
// ✓ Thread created automatically
// ✗ No control over execution timing

// METHOD 2: std::promise (Manual Value Setting)
std::promise<int> prom;
auto fut = prom.get_future();
// ... later ...
prom.set_value(result);  // Manually set value
// ✓ Full control over value
// ✗ No function wrapping

// METHOD 3: std::packaged_task (Manual Execution)
std::packaged_task<int(int)> task(compute);
auto fut = task.get_future();
// ... later, when YOU want ...
task(5);  // Execute NOW
// ✓ Wraps function
// ✓ Control execution timing
// ✓ Can pass to threads
```

### Comparison Table:

| Feature | async | promise | packaged_task |
|---------|-------|---------|---------------|
| **Type** | Function | Class | Class |
| **Wraps function?** | Yes (executes) | No | Yes (stores) |
| **Execution** | Automatic | N/A | Manual |
| **Creates thread?** | Yes (optional) | No | No |
| **Control level** | Low | High | Medium |
| **Use case** | Quick parallel | Manual value | Task queue, deferred |

---

## 📝 Code Examples

### Example 1: Basic Usage

```cpp
#include <iostream>
#include <future>

int square(int x) {
    return x * x;
}

int main() {
    // Create task
    std::packaged_task<int(int)> task(square);
    
    // Get future BEFORE execution
    std::future<int> fut = task.get_future();
    
    // Execute when ready
    task(7);
    
    // Get result
    std::cout << "Result: " << fut.get() << std::endl;  // 49
    
    return 0;
}
```

### Example 2: With Lambda

```cpp
std::packaged_task<int(int, int)> task([](int a, int b) {
    return a + b;
});

auto fut = task.get_future();
task(10, 20);
std::cout << fut.get() << std::endl;  // 30
```

### Example 3: Execute in Thread

```cpp
std::packaged_task<int(int)> task([](int x) {
    return x * 2;
});

std::future<int> fut = task.get_future();

// Move task to thread
std::thread t(std::move(task), 10);

std::cout << fut.get() << std::endl;  // 20
t.join();
```

### Example 4: Reset & Reuse

```cpp
std::packaged_task<int(int)> task([](int x) {
    return x + 1;
});

// First use
auto fut1 = task.get_future();
task(5);
std::cout << fut1.get() << std::endl;  // 6

// Reset and reuse
task.reset();  // Get new shared state

auto fut2 = task.get_future();
task(10);
std::cout << fut2.get() << std::endl;  // 11
```

### Example 5: Exception Handling

```cpp
std::packaged_task<int(int)> task([](int x) {
    if (x < 0) {
        throw std::invalid_argument("Negative!");
    }
    return x * x;
});

auto fut = task.get_future();
task(-5);  // Throws exception

try {
    int result = fut.get();  // Exception re-thrown here
} catch (const std::invalid_argument& e) {
    std::cout << "Error: " << e.what() << std::endl;
}
```

### Example 6: Task Queue Pattern

```cpp
#include <queue>
#include <thread>

std::queue<std::packaged_task<void()>> task_queue;

// Add tasks to queue
auto task1 = std::packaged_task<int()>([]() { return 42; });
auto fut1 = task1.get_future();
task_queue.push(std::move(task1));

// Worker thread processes tasks
std::thread worker([&]() {
    while (!task_queue.empty()) {
        auto task = std::move(task_queue.front());
        task_queue.pop();
        task();  // Execute
    }
});

int result = fut1.get();
worker.join();
```

---

## 🎓 When to Use packaged_task?

### ✅ Use `packaged_task` when:
- You want to wrap a function for later execution
- You need to pass tasks to a task queue
- You want manual control over execution timing
- Building a thread pool or work queue system
- Need to store tasks in containers
- Want to reuse tasks (with `reset()`)

### ❌ Use `std::async` instead when:
- You want immediate automatic execution
- Simple parallelism is all you need
- Don't need manual execution control

### ❌ Use `std::promise` instead when:
- No function to wrap (just setting a value)
- Value comes from multiple code paths
- Need very low-level control

---

## 🔍 Internal Structure

```
packaged_task<int(int)>
┌────────────────────────────────────┐
│                                    │
│  ┌─────────────────────────┐       │
│  │  Wrapped Function       │       │
│  │  (function/lambda)      │       │
│  └─────────────────────────┘       │
│              ↓                     │
│  ┌─────────────────────────┐       │
│  │  Internal Promise       │       │
│  │  (stores result)        │       │
│  └─────────────────────────┘       │
│              ↓                     │
│  ┌─────────────────────────┐       │
│  │  Shared State           │ ◄─────┼─── future<int>
│  └─────────────────────────┘       │
│                                    │
└────────────────────────────────────┘
```

---

## 🚀 Common Patterns

### Pattern 1: Task Queue
```cpp
std::queue<std::packaged_task<void()>> tasks;

// Producer adds tasks
tasks.push(std::move(task));

// Worker processes tasks
while (!tasks.empty()) {
    auto task = std::move(tasks.front());
    tasks.pop();
    task();
}
```

### Pattern 2: Thread Pool
```cpp
class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::packaged_task<void()>> tasks;
    
    void worker() {
        while (true) {
            std::packaged_task<void()> task;
            // Get task from queue
            task();  // Execute
        }
    }
};
```

### Pattern 3: Deferred Execution
```cpp
std::vector<std::packaged_task<int()>> deferred_tasks;

// Store tasks for later
deferred_tasks.push_back(std::move(task1));
deferred_tasks.push_back(std::move(task2));

// Execute all when ready
for (auto& task : deferred_tasks) {
    task();
}
```

---

## ⚡ Key Characteristics

| Property | Value |
|----------|-------|
| **Copyable** | ❌ No (move-only) |
| **One-time use** | ❌ No (can reset()) |
| **Thread-safe** | ✅ Yes (internal promise/future) |
| **Exception safe** | ✅ Yes (propagates to future) |
| **Reusable** | ✅ Yes (with reset()) |
| **Creates threads** | ❌ No (you control execution) |
| **Header** | `<future>` |

---

## 📖 Complete File Reference

### Main Example File
**06_packaged_task_complete.cpp** (~600 lines)

Covers:
1. ✅ Basic concept and understanding
2. ✅ Template signature explained
3. ✅ All member functions with examples
4. ✅ Comparison with async and promise
5. ✅ Different callable types
6. ✅ Real-world task queue implementation
7. ✅ Exception handling
8. ✅ Move semantics

### Quick Reference
**PACKAGED_TASK_REFERENCE.cpp** (~500 lines)

Includes:
- Visual diagrams
- API reference
- Code snippets
- Pattern examples
- When to use guide

---

## 🔗 Summary

**std::packaged_task** is the "middle ground" between:
- **High-level** `std::async` (automatic execution)
- **Low-level** `std::promise` (manual value setting)

It gives you:
- ✅ Function wrapping
- ✅ Automatic future creation
- ✅ Manual execution control
- ✅ Perfect for task queues and thread pools

**Remember:** 
- `async` = "Do it now automatically"
- `promise` = "I'll set the value manually"
- `packaged_task` = "I'll run this function when I want"

---

## 🛠️ Build & Run

```bash
# Navigate to directory
cd /home/z00542kh/cpp_KT/C++_KT/Future/async

# Build
mkdir -p build && cd build
cmake ..
cmake --build .

# Run example
./bin/06_packaged_task_complete
```

**Estimated runtime:** ~10 seconds

---

**Created:** December 1, 2025  
**C++ Standard:** C++17  
**Complete coverage of std::packaged_task**
