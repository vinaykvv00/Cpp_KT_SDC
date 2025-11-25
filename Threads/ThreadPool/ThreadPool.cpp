// ThreadPoolDemo.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <utility> 

class ThreadPool
{
public:
    explicit ThreadPool(size_t numThreads) : stop(false)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this] {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return; 

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template <class F>
    void enqueue(F&& task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread &worker : workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
    std::queue<std::function<void()>> tasks;
};

int main()
{
    ThreadPool pool(4);

    std::cout << "creating thread pool with 4 threads\n";
    std::cout << "enqueue: assign some tasks to the thread pool\n";

    for (int i = 0; i < 8; ++i)
    {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is starting on thread " << std::this_thread::get_id() << '\n';
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " is completed on thread " << std::this_thread::get_id() << '\n';
        });
    }

    std::cout << "main thread: all tasks enqueued, continuing other work\n";
    return 0;
}
