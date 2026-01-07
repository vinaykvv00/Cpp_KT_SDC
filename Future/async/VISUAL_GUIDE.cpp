/**
 * VISUAL GUIDE: std::future, std::async, std::promise Relationships
 * ==================================================================
 */

/*
 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    WHAT ARE THEY? (TYPES)                                  ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   std::future<T>          std::async              std::promise<T>
   ┌──────────────┐        ┌──────────────┐        ┌──────────────┐
   │ CLASS        │        │ FUNCTION     │        │ CLASS        │
   │ TEMPLATE     │        │ TEMPLATE     │        │ TEMPLATE     │
   └──────────────┘        └──────────────┘        └──────────────┘
        ↓                        ↓                        ↓
   Receives result         Runs function           Sets result
   (Consumer)              (Executor)              (Producer)


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    HOW THEY WORK TOGETHER                                  ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   METHOD 1: Using std::async (HIGH-LEVEL, AUTOMATIC)
   ═══════════════════════════════════════════════════

       std::async(func, args...)
              │
              ├─► Creates thread (optional)
              ├─► Runs function
              └─► Returns std::future<T>
                        │
                        └─► future.get() ──► Result


   METHOD 2: Using std::promise (LOW-LEVEL, MANUAL)
   ════════════════════════════════════════════════

       std::promise<T> prom;
              │
              ├─► prom.get_future() ──► std::future<T>
              │                              │
       Worker Thread:                        │
       prom.set_value(x)                     │
              │                              │
              └──────────────────────────────┤
                                             │
                                    future.get() ──► Result


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    DATA FLOW DIAGRAM                                       ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   PRODUCER SIDE          SHARED STATE           CONSUMER SIDE
   ─────────────          ────────────           ─────────────

   std::promise<T>  ───►  [Value/Except]  ◄───  std::future<T>
        │                       │                      │
        │                       │                      │
   set_value(x) ────►      Set Value      ◄──── get()
   set_exception() ──►   Set Exception    ◄──── get()
                              │                  │
                              │                  wait()
                              │                  wait_for()
                              │                  wait_until()


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    STATE TRANSITIONS                                       ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   std::future State:
   
   [Created]  ───────►  [Valid]  ───────►  [Result Ready]  ───get()──►  [Invalid]
      │                    │                      │
      │                valid()                wait() ◄─┐
      │                  = false              wait_for()│
      │                                       wait_until()
      │
      └─► [Invalid from start] (default constructor)


   std::promise State:

   [Created] ──get_future()──► [Has Future] ──set_value()──► [Value Set]
      │                              │
      │                              └──set_exception()──► [Exception Set]
      │
      └──► Can only set ONCE!


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    THREAD INTERACTION                                      ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   SCENARIO 1: std::async with std::launch::async
   ═══════════════════════════════════════════════

   Main Thread                           Worker Thread
   ───────────                           ─────────────
       │
       │ fut = async(func, args)
       ├─────────────────────────────────► Thread Created
       │                                         │
       │ doing other work...                    │ func(args)
       │                                         │ computing...
       │                                         │
       │ result = fut.get()                     │
       │─────────────BLOCKS─────────────────────┤
       │                                         │ return value
       │ ◄───────────────────────────────────────┤
       │                                    Thread Exits
       │ continue with result
       ▼


   SCENARIO 2: std::promise/future
   ═══════════════════════════════

   Main Thread                           Worker Thread
   ───────────                           ─────────────
       │
       │ promise<T> prom
       │ future<T> fut = prom.get_future()
       │
       │ thread t([prom](){...})
       ├─────────────────────────────────► Thread Created
       │                                         │
       │ result = fut.get()                     │ working...
       │─────────────BLOCKS─────────────────────┤
       │                                         │
       │                                         │ prom.set_value(x)
       │ ◄───────────────────────────────────────┤
       │                                         │
       │ continue with result                    │ continue
       │                                         ▼
       │ t.join() ─────────────────────────► Thread Exits
       ▼


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    EXCEPTION FLOW                                          ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   With std::async:
   
   Worker Thread                         Main Thread
   ─────────────                         ───────────
       │
       │ throw exception
       │────────►  [Stored in shared state]
                        │
                        │ future.get()
                        └────────► Exception re-thrown
                                         │
                                    try-catch handles


   With std::promise:

   Worker Thread                         Main Thread
   ─────────────                         ───────────
       │
       │ catch (...)
       │ prom.set_exception(
       │   std::current_exception()
       │ )
       │────────►  [Stored in shared state]
                        │
                        │ future.get()
                        └────────► Exception re-thrown
                                         │
                                    try-catch handles


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    SPECIALIZATIONS                                         ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   ┌────────────────────┬──────────────────────┬────────────────────────────┐
   │ Specialization     │ Use Case             │ Example                    │
   ├────────────────────┼──────────────────────┼────────────────────────────┤
   │ future<T>          │ Return value         │ future<int> fut            │
   │                    │                      │ int result = fut.get()     │
   ├────────────────────┼──────────────────────┼────────────────────────────┤
   │ future<T&>         │ Return reference     │ future<int&> fut           │
   │                    │                      │ int& ref = fut.get()       │
   ├────────────────────┼──────────────────────┼────────────────────────────┤
   │ future<void>       │ Just completion      │ future<void> fut           │
   │                    │ signal               │ fut.get() // wait          │
   └────────────────────┴──────────────────────┴────────────────────────────┘


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    DECISION TREE: WHICH TO USE?                            ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   START: Need asynchronous operation?
      │
      ├─► Yes, simple function to run?
      │        │
      │        └─► Use std::async
      │                │
      │                ├─► Need guaranteed thread? → std::launch::async
      │                └─► Want lazy evaluation? → std::launch::deferred
      │
      ├─► Yes, need manual control?
      │        │
      │        └─► Use std::promise
      │                │
      │                ├─► Multiple code paths? → promise.set_value()
      │                └─► Synchronization? → promise<void>
      │
      └─► Just need to receive result?
               │
               └─► Use std::future
                       │
                       ├─► Multiple consumers? → future.share()
                       ├─► Check timeout? → future.wait_for()
                       └─► Just get result? → future.get()


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    LIFETIME & OWNERSHIP                                    ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   std::promise<T>  ◄────────────►  std::future<T>
        │                                  │
        │        SHARED STATE              │
        └────────►  [VALUE]  ◄─────────────┘
                      │
                      │ Reference Counted
                      │ Destroyed when both sides gone
                      ▼

   MOVE SEMANTICS:
   ───────────────
   promise1 ──move──► promise2    (promise1 now invalid)
   future1  ──move──► future2     (future1 now invalid)

   NOT COPYABLE:
   ─────────────
   promise2 = promise1    ✗ ERROR
   future2  = future1     ✗ ERROR


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    COMMON PATTERNS                                         ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   PATTERN 1: Parallel For-Each
   ════════════════════════════
   
   for (item : items)  ──async(process, item)──►  futures[]
                                                       │
                                                   for (fut : futures)
                                                       │
                                                   results[] ← fut.get()


   PATTERN 2: Producer-Consumer
   ════════════════════════════

   Producer Thread          Consumer Thread
        │                         │
   promise<T> prom     ←─────►  future<T> fut
        │                         │
   produce data()                 │
        │                         │
   prom.set_value(data)          │
        │                         │
        └───────────────────►  fut.get()
                                  │
                              consume(data)


   PATTERN 3: Synchronization Barrier
   ══════════════════════════════════

        promise<void> barrier
               │
        barrier.get_future().share()
               │
               ├──────► Thread 1: shared_fut.get() → WORK
               ├──────► Thread 2: shared_fut.get() → WORK
               └──────► Thread 3: shared_fut.get() → WORK
               │
        barrier.set_value()  (releases all)


   PATTERN 4: Timeout Pattern
   ═════════════════════════

        future = async(task)
               │
        wait_for(timeout)
               │
          ┌────┴────┐
          │         │
        READY    TIMEOUT
          │         │
        get()   handle timeout


 ╔════════════════════════════════════════════════════════════════════════════╗
 ║                    SUMMARY TABLE                                           ║
 ╚════════════════════════════════════════════════════════════════════════════╝

   ┌──────────┬─────────────┬─────────────┬────────────┬──────────────────┐
   │ Feature  │ future      │ async       │ promise    │ shared_future    │
   ├──────────┼─────────────┼─────────────┼────────────┼──────────────────┤
   │ Type     │ Class       │ Function    │ Class      │ Class            │
   │ Role     │ Consumer    │ Executor    │ Producer   │ Multi-consumer   │
   │ Copyable │ NO          │ N/A         │ NO         │ YES              │
   │ get()    │ Once        │ Returns fut │ N/A        │ Multiple times   │
   │ set()    │ N/A         │ Automatic   │ Once       │ N/A              │
   │ Thread   │ No          │ Yes/No      │ No         │ No               │
   │ Level    │ High/Low    │ High        │ Low        │ High/Low         │
   └──────────┴─────────────┴─────────────┴────────────┴──────────────────┘

*/

// Compile this file to check syntax (it's a valid C++ file with comments)
int main() {
    return 0;
}
