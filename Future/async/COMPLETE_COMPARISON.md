# Complete Comparison: future, async, promise, packaged_task

## 🎯 Quick Decision Tree

```
Need asynchronous operation?
│
├─► Simple parallel execution?
│   └─► Use std::async ✅
│       Example: auto fut = std::async(func, args);
│
├─► Need to set value manually from different code paths?
│   └─► Use std::promise ✅
│       Example: promise.set_value(result);
│
├─► Need to wrap function and control when it executes?
│   └─► Use std::packaged_task ✅
│       Example: task(args); // execute when ready
│
└─► Just need to receive result?
    └─► Use std::future ✅
        Example: result = fut.get();
```

---

## 📊 Complete Comparison Table

| Feature | future | async | promise | packaged_task |
|---------|--------|-------|---------|---------------|
| **Type** | Class Template | Function Template | Class Template | Class Template |
| **Purpose** | Receive result | Execute function | Set value manually | Wrap function for later |
| **Header** | `<future>` | `<future>` | `<future>` | `<future>` |
| **Copyable** | ❌ No | N/A | ❌ No | ❌ No |
| **Move-only** | ✅ Yes | N/A | ✅ Yes | ✅ Yes |
| **Creates threads** | ❌ No | ✅ Yes (optional) | ❌ No | ❌ No |
| **Execution** | N/A | Automatic | N/A | Manual |
| **Wraps function** | ❌ No | ✅ Yes | ❌ No | ✅ Yes |
| **Control level** | Consumer | Low | High | Medium |
| **One-time use** | ✅ Yes (get()) | N/A | ✅ Yes (set) | ❌ No (reset()) |
| **Reusable** | ❌ No | N/A | ❌ No | ✅ Yes (reset()) |
| **Exception safe** | ✅ Yes | ✅ Yes | ✅ Yes | ✅ Yes |
| **Use case** | Get results | Quick parallel | Manual control | Task queue |

---

## 💻 Side-by-Side Code Examples

### Same Task, Four Ways:

```cpp
// Task: Compute square of a number asynchronously

int compute_square(int x) {
    return x * x;
}

// ============================================================================
// METHOD 1: Using std::async (HIGH-LEVEL, AUTOMATIC)
// ============================================================================
std::future<int> fut = std::async(std::launch::async, compute_square, 5);
// ✓ Function starts IMMEDIATELY
// ✓ Thread created automatically
// ✓ Simple and clean
// ✗ No control over when it runs
int result = fut.get();  // 25

// ============================================================================
// METHOD 2: Using std::promise (LOW-LEVEL, MANUAL VALUE)
// ============================================================================
std::promise<int> prom;
std::future<int> fut = prom.get_future();

std::thread t([](std::promise<int> p) {
    int result = compute_square(5);
    p.set_value(result);  // Manually set value
}, std::move(prom));

// ✓ Full control over value setting
// ✓ Can set from anywhere in code
// ✗ No function wrapping
// ✗ More verbose
int result = fut.get();  // 25
t.join();

// ============================================================================
// METHOD 3: Using std::packaged_task (MEDIUM-LEVEL, MANUAL EXECUTION)
// ============================================================================
std::packaged_task<int(int)> task(compute_square);
std::future<int> fut = task.get_future();

// Execute when YOU want
task(5);  // Function runs NOW
// OR pass to thread:
// std::thread t(std::move(task), 5);

// ✓ Wraps function
// ✓ Control when it executes
// ✓ Can pass to thread or call directly
// ✓ Reusable with reset()
int result = fut.get();  // 25

// ============================================================================
// METHOD 4: Just std::future (CONSUMER ONLY)
// ============================================================================
// future is what you GET from the above methods
std::future<int> fut;  // Invalid by default
// Must get from async, promise, or packaged_task

// ✓ Simple interface to get result
// ✓ Can wait with timeout
// ✗ Can't create standalone
int result = fut.get();
```

---

## 🔄 Data Flow Diagrams

### Method 1: std::async
```
std::async(func, args)
    │
    ├─► Creates thread (if launch::async)
    ├─► Executes function
    └─► Returns future<T>
             │
        future.get() ──► Result
```

### Method 2: std::promise
```
promise<T> prom
    │
    ├─► prom.get_future() ──► future<T>
    │                              │
    │                              │
Worker Thread:                     │
prom.set_value(result)            │
    │                              │
    └──────────────────────────────┤
                                   │
                          future.get() ──► Result
```

### Method 3: std::packaged_task
```
packaged_task<T(Args)> task(func)
    │
    ├─► task.get_future() ──► future<T>
    │                              │
    │                              │
task(args)  ◄──── YOU CONTROL      │
    │                              │
    └──────────────────────────────┤
                                   │
                          future.get() ──► Result
```

---

## 📈 Complexity & Control Levels

```
Low Control                                              High Control
    ↓                                                         ↓
┌──────────┬──────────────────┬──────────────────┬──────────────┐
│  async   │  packaged_task   │                  │   promise    │
│          │                  │                  │              │
│ Easy     │ Medium           │                  │ Complex      │
│ Auto     │ Manual execution │                  │ Manual value │
└──────────┴──────────────────┴──────────────────┴──────────────┘
```

---

## 🎓 When to Use Each

### Use `std::async` when:
```cpp
✅ Quick parallelism needed
✅ Simple one-off async operations  
✅ Don't need execution control
✅ Want high-level abstraction

Example use cases:
- Parallel data processing
- Independent computations
- Quick background tasks
```

### Use `std::promise` when:
```cpp
✅ Manual value setting required
✅ Value comes from different code paths
✅ Building custom async patterns
✅ Need low-level control

Example use cases:
- Producer-consumer patterns
- Synchronization barriers
- Custom async implementations
- Setting values conditionally
```

### Use `std::packaged_task` when:
```cpp
✅ Wrap function for later execution
✅ Building task queues
✅ Thread pool implementations
✅ Need to store tasks in containers
✅ Want to reuse tasks

Example use cases:
- Task queue systems
- Thread pools
- Deferred execution
- Work scheduling systems
```

### Use `std::future` for:
```cpp
✅ Receiving results (from any method)
✅ Checking if result is ready
✅ Waiting with timeout
✅ Getting exceptions from other threads

Example use cases:
- Getting async results
- Timeout handling
- Exception propagation
- Result synchronization
```

---

## 📝 Feature Matrix

### Getting a Future

| Method | How to get future | Can get multiple? |
|--------|-------------------|-------------------|
| **async** | Returned by async() | No (move-only) |
| **promise** | prom.get_future() | No (once per promise) |
| **packaged_task** | task.get_future() | No (once per task, unless reset) |

### Setting a Value/Result

| Method | How to set | When set |
|--------|-----------|----------|
| **async** | Automatic | When function returns |
| **promise** | prom.set_value() | Manually, anytime |
| **packaged_task** | task(args) | When task is called |

### Execution Control

| Method | When executes | Control |
|--------|---------------|---------|
| **async** | Immediately (async) or on get() (deferred) | Low |
| **promise** | N/A (no function) | High (manual) |
| **packaged_task** | When you call task() | Medium |

---

## 🔍 Internal Structure Comparison

```
std::async
┌─────────────────────────────┐
│ Creates thread (optional)   │
│         │                   │
│         ↓                   │
│ Executes function           │
│         │                   │
│         ↓                   │
│ Returns future<T> ─────────►│
└─────────────────────────────┘


std::promise
┌─────────────────────────────┐
│ promise<T>                  │
│    │                        │
│    ├─► Shared State ◄───────┼── future<T>
│    │                        │
│    └─► set_value()          │
└─────────────────────────────┘


std::packaged_task
┌─────────────────────────────┐
│ Wrapped Function            │
│         │                   │
│         ↓                   │
│ Internal Promise            │
│         │                   │
│         └─► Shared State ◄──┼── future<T>
│                             │
│ Execute: task(args)         │
└─────────────────────────────┘
```

---

## 💡 Real-World Examples

### Example 1: Web Server Requests

```cpp
// Using async for independent requests
std::vector<std::future<Response>> responses;

for (auto& request : requests) {
    responses.push_back(
        std::async(std::launch::async, handle_request, request)
    );
}

// Collect all responses
for (auto& fut : responses) {
    process_response(fut.get());
}
```

### Example 2: Producer-Consumer

```cpp
// Using promise for producer-consumer
std::promise<Data> data_promise;
std::future<Data> data_future = data_promise.get_future();

// Producer thread
std::thread producer([](std::promise<Data> p) {
    Data data = generate_data();
    p.set_value(std::move(data));
}, std::move(data_promise));

// Consumer thread
Data data = data_future.get();
process_data(data);

producer.join();
```

### Example 3: Task Queue

```cpp
// Using packaged_task for task queue
std::queue<std::packaged_task<void()>> task_queue;

// Add tasks
for (int i = 0; i < 10; ++i) {
    std::packaged_task<int(int)> task(process);
    auto fut = task.get_future();
    futures.push_back(std::move(fut));
    task_queue.push([task = std::move(task), i]() mutable { task(i); });
}

// Worker processes tasks
while (!task_queue.empty()) {
    auto task = std::move(task_queue.front());
    task_queue.pop();
    task();
}
```

---

## ⚠️ Common Mistakes

### Mistake 1: Calling get() Twice
```cpp
// ❌ WRONG
std::future<int> fut = std::async(func);
int result1 = fut.get();
int result2 = fut.get();  // ERROR! Can only call once

// ✅ CORRECT - Use shared_future
std::shared_future<int> shared = std::async(func).share();
int result1 = shared.get();
int result2 = shared.get();  // OK!
```

### Mistake 2: Using After Move
```cpp
// ❌ WRONG
std::packaged_task<int()> task1(func);
std::packaged_task<int()> task2 = std::move(task1);
task1();  // ERROR! task1 is invalid after move

// ✅ CORRECT
std::packaged_task<int()> task1(func);
std::packaged_task<int()> task2 = std::move(task1);
task2();  // Use task2
```

### Mistake 3: Forgetting to Execute packaged_task
```cpp
// ❌ WRONG
std::packaged_task<int()> task(func);
auto fut = task.get_future();
int result = fut.get();  // HANGS! Task never executed

// ✅ CORRECT
std::packaged_task<int()> task(func);
auto fut = task.get_future();
task();  // Execute the task
int result = fut.get();
```

---

## 📚 Summary Cheat Sheet

```cpp
// ============================================================================
// QUICK REFERENCE
// ============================================================================

// HIGH-LEVEL: Use async for simple parallelism
auto fut = std::async(std::launch::async, func, args);
result = fut.get();

// LOW-LEVEL: Use promise for manual value setting
std::promise<T> prom;
auto fut = prom.get_future();
prom.set_value(value);
result = fut.get();

// MEDIUM-LEVEL: Use packaged_task for controlled execution
std::packaged_task<T(Args)> task(func);
auto fut = task.get_future();
task(args);  // Execute when ready
result = fut.get();

// CONSUMER: future gets result from any of above
std::future<T> fut;  // From async/promise/packaged_task
T result = fut.get();
```

---

## 🎯 Final Recommendation

**Choose based on your needs:**

1. **Need it simple and automatic?** → `std::async`
2. **Need full manual control?** → `std::promise`  
3. **Need to schedule task execution?** → `std::packaged_task`
4. **Just getting results?** → `std::future`

**Most common:** `std::async` for 80% of use cases  
**Advanced patterns:** `std::packaged_task` for task queues  
**Custom implementations:** `std::promise` for maximum control

---

**All four work together to provide complete async programming in C++!**
