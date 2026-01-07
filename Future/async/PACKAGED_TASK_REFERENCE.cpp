/**
 * PACKAGED_TASK VISUAL GUIDE & QUICK REFERENCE
 * ============================================
 */

#include <future>
#include <functional>

/*
 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    WHAT IS std::packaged_task?                             ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   TYPE: CLASS TEMPLATE (not function or keyword)
   
   THINK OF IT AS: A "task package" that combines:
   ┌─────────────────────────────────────────────────────────────┐
   │  1. A wrapped function/callable                             │
   │  2. A promise (to store the result)                         │
   │  3. A future (to retrieve the result)                       │
   │  4. Manual execution control (YOU decide when it runs)      │
   └─────────────────────────────────────────────────────────────┘


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    TEMPLATE SIGNATURE                                      ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   std::packaged_task<ReturnType(ParameterTypes...)>
                       ↑           ↑
                       |           └─ Parameter types
                       └─ Return type

   EXAMPLES:
   
   std::packaged_task<int(int)>                    // int func(int)
   std::packaged_task<void()>                      // void func()
   std::packaged_task<double(int, double)>         // double func(int, double)
   std::packaged_task<std::string(std::string)>    // string func(string)


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    HOW IT WORKS (VISUAL)                                   ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   STEP-BY-STEP FLOW:
   
   1. CREATE TASK
      ┌──────────────────────────────────────┐
      │ packaged_task<int(int)> task(func)   │
      │                                      │
      │  Wrapped Function: func              │
      │  Internal Promise: created           │
      │  Status: NOT executed yet            │
      └──────────────────────────────────────┘
   
   2. GET FUTURE
      ┌──────────────────────────────────────┐
      │ future<int> fut = task.get_future()  │
      │                                      │
      │  Future connected to internal promise│
      └──────────────────────────────────────┘
   
   3. EXECUTE TASK (you control when!)
      ┌──────────────────────────────────────┐
      │ task(5);  ← You call it manually     │
      │                                      │
      │  Function executes: func(5)          │
      │  Result stored in internal promise   │
      └──────────────────────────────────────┘
   
   4. GET RESULT
      ┌──────────────────────────────────────┐
      │ int result = fut.get();              │
      │                                      │
      │  Retrieves result from promise       │
      └──────────────────────────────────────┘


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    INTERNAL STRUCTURE                                      ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   packaged_task<int(int)>
   ┌────────────────────────────────────────────────┐
   │                                                │
   │  ┌──────────────────────────────┐             │
   │  │  Wrapped Callable            │             │
   │  │  (function/lambda/functor)   │             │
   │  └──────────────────────────────┘             │
   │              │                                 │
   │              ↓                                 │
   │  ┌──────────────────────────────┐             │
   │  │  Internal Promise<int>        │             │
   │  │  (stores result)              │             │
   │  └──────────────────────────────┘             │
   │              │                                 │
   │              ↓                                 │
   │  ┌──────────────────────────────┐             │
   │  │  Shared State                │             │
   │  │  (connects promise & future)  │             │
   │  └──────────────────────────────┘             │
   │              ↑                                 │
   └──────────────┼─────────────────────────────────┘
                  │
                  └────► future<int> (external)


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    MEMBER FUNCTIONS                                        ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   ┌────────────────────────┬──────────────────────────────────────────────┐
   │ Function               │ Description                                  │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ Constructor            │ Create from callable                         │
   │ packaged_task(func)    │                                              │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ get_future()           │ Get associated future (once only!)           │
   │ future<T> get_future() │ Throws if called twice                       │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ operator()             │ Execute the wrapped function                 │
   │ void operator()(Args)  │ Result stored automatically                  │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ valid()                │ Check if task has shared state               │
   │ bool valid()           │ Returns false after move                     │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ reset()                │ Reset for reuse (new shared state)           │
   │ void reset()           │ Can execute again after reset                │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ swap()                 │ Swap two tasks                               │
   │ void swap(task)        │                                              │
   ├────────────────────────┼──────────────────────────────────────────────┤
   │ make_ready_at_thread_  │ Execute, result ready at thread exit         │
   │ exit()                 │ Like set_value_at_thread_exit for promise   │
   └────────────────────────┴──────────────────────────────────────────────┘


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    COMPARISON CHART                                        ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   ┌──────────────┬─────────────────┬─────────────────┬──────────────────┐
   │ Feature      │ async           │ promise         │ packaged_task    │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Type         │ Function        │ Class           │ Class            │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Wraps func?  │ Yes (executes)  │ No              │ Yes (stores)     │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Execution    │ Automatic       │ N/A             │ Manual           │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Control      │ Low             │ High            │ Medium           │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Thread?      │ Creates thread  │ No thread       │ No thread        │
   ├──────────────┼─────────────────┼─────────────────┼──────────────────┤
   │ Use case     │ Quick parallel  │ Manual value    │ Task queue       │
   │              │ execution       │ setting         │ Deferred exec    │
   └──────────────┴─────────────────┴─────────────────┴──────────────────┘


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    EXECUTION MODELS                                        ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   MODEL 1: Direct Execution (Main Thread)
   ════════════════════════════════════════
   
   packaged_task<int(int)> task(func);
   future<int> fut = task.get_future();
   task(5);  ← Executes NOW on this thread
   int result = fut.get();


   MODEL 2: Thread Execution (Worker Thread)
   ═════════════════════════════════════════
   
   packaged_task<int(int)> task(func);
   future<int> fut = task.get_future();
   
   thread t(std::move(task), 5);  ← Task moved to thread
   // Task executes in thread
   int result = fut.get();
   t.join();


   MODEL 3: Deferred Execution (Later)
   ═══════════════════════════════════
   
   packaged_task<int(int)> task(func);
   future<int> fut = task.get_future();
   
   // ... do other work ...
   
   task(5);  ← Execute when ready
   int result = fut.get();


   MODEL 4: Task Queue (Work Queue Pattern)
   ════════════════════════════════════════
   
   queue<packaged_task<void()>> task_queue;
   
   // Producer: adds tasks
   task_queue.push(std::move(task));
   
   // Worker: processes tasks
   while (!queue.empty()) {
       auto task = queue.front();
       queue.pop();
       task();  ← Execute task
   }


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    CODE EXAMPLES                                           ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   EXAMPLE 1: Basic Usage
   ══════════════════════
   
   int compute(int x) { return x * x; }
   
   packaged_task<int(int)> task(compute);
   future<int> fut = task.get_future();
   task(5);  // Execute
   cout << fut.get();  // 25


   EXAMPLE 2: With Lambda
   ══════════════════════
   
   packaged_task<int(int, int)> task([](int a, int b) {
       return a + b;
   });
   future<int> fut = task.get_future();
   task(10, 20);
   cout << fut.get();  // 30


   EXAMPLE 3: In Thread
   ════════════════════
   
   packaged_task<int(int)> task([](int x) { return x * 2; });
   future<int> fut = task.get_future();
   
   thread t(std::move(task), 7);
   cout << fut.get();  // 14
   t.join();


   EXAMPLE 4: Reset & Reuse
   ════════════════════════
   
   packaged_task<int(int)> task([](int x) { return x + 1; });
   
   // First use
   auto fut1 = task.get_future();
   task(5);
   cout << fut1.get();  // 6
   
   // Reset and reuse
   task.reset();
   auto fut2 = task.get_future();
   task(10);
   cout << fut2.get();  // 11


   EXAMPLE 5: Exception Handling
   ══════════════════════════════
   
   packaged_task<int(int)> task([](int x) {
       if (x < 0) throw invalid_argument("Negative!");
       return x * x;
   });
   
   future<int> fut = task.get_future();
   task(-5);
   
   try {
       int result = fut.get();  // Exception re-thrown
   } catch (const invalid_argument& e) {
       cout << e.what();
   }


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    WHEN TO USE packaged_task?                              ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   USE packaged_task WHEN:
   ✓ You want to wrap a function for later execution
   ✓ You need to pass tasks to a task queue
   ✓ You want manual control over execution timing
   ✓ Building a thread pool or work queue system
   ✓ Need to store tasks in containers
   ✓ Want to reuse tasks (with reset())

   DON'T USE (use std::async instead) WHEN:
   ✗ You want immediate automatic execution
   ✗ Simple parallelism is all you need
   ✗ Don't need manual execution control

   DON'T USE (use std::promise instead) WHEN:
   ✗ No function to wrap (just setting a value)
   ✗ Value comes from multiple code paths
   ✗ Need very low-level control


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    COMMON PATTERNS                                         ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   PATTERN 1: Task Queue
   ═════════════════════
   
   queue<packaged_task<void()>> tasks;
   
   // Add task
   packaged_task<int(int)> task(func);
   future<int> fut = task.get_future();
   tasks.push(std::move(task));
   
   // Process tasks
   while (!tasks.empty()) {
       auto t = std::move(tasks.front());
       tasks.pop();
       t();
   }


   PATTERN 2: Thread Pool
   ══════════════════════
   
   class ThreadPool {
       vector<thread> workers;
       queue<packaged_task<void()>> tasks;
       
       void worker() {
           while (true) {
               packaged_task<void()> task;
               // Get task from queue
               task = std::move(tasks.front());
               tasks.pop();
               task();  // Execute
           }
       }
   };


   PATTERN 3: Deferred Execution
   ═════════════════════════════
   
   vector<packaged_task<int()>> deferred_tasks;
   
   // Store tasks
   deferred_tasks.push_back(packaged_task<int()>(task1));
   deferred_tasks.push_back(packaged_task<int()>(task2));
   
   // Execute later
   for (auto& task : deferred_tasks) {
       task();
   }


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    KEY CHARACTERISTICS                                     ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   ✓ Move-Only      Cannot be copied, only moved
   ✓ One-Time Use   operator() can be called once (unless reset())
   ✓ Thread-Safe    Internal promise/future are thread-safe
   ✓ Exception Safe Exceptions propagate through future.get()
   ✓ Reusable       Can reset() for reuse with new future
   ✓ Flexible       Works with any callable type
   ✓ No Threads     Doesn't create threads (you control execution)


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    COMPLETE API REFERENCE                                  ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   // Constructor
   packaged_task();                           // Default (invalid)
   packaged_task(Function&& f);               // From callable
   packaged_task(packaged_task&& other);      // Move constructor

   // Assignment
   packaged_task& operator=(packaged_task&& other);  // Move assignment

   // Execution
   void operator()(Args... args);              // Execute wrapped function

   // Future Management
   future<R> get_future();                    // Get future (once only)

   // State
   bool valid() const noexcept;               // Check if has shared state

   // Reset
   void reset();                              // Reset for reuse

   // Swap
   void swap(packaged_task& other);           // Swap tasks

   // Thread Exit
   void make_ready_at_thread_exit(Args...);   // Execute, ready at exit


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    COMPILE & BUILD                                         ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   Header:
   #include <future>

   Compile:
   g++ -std=c++17 -pthread program.cpp -o program

   CMake:
   find_package(Threads REQUIRED)
   target_link_libraries(target PRIVATE Threads::Threads)

*/

int main() { return 0; }
