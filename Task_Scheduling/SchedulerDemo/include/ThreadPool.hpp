#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t workers = std::thread::hardware_concurrency())
        : stop_(false) {
        if (workers == 0) workers = 1;
        for (std::size_t i = 0; i < workers; ++i) {
            threads_.emplace_back([this] { this->workerLoop(); });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto &t : threads_) {
            if (t.joinable()) t.join();
        }
    }

    template <class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        using R = typename std::invoke_result<F, Args...>::type;
        auto task = std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::lock_guard<std::mutex> lk(mtx_);
            tasks_.emplace([task]{ (*task)(); });
        }
        cv_.notify_one();
        return task->get_future();
    }

private:
    void workerLoop() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lk(mtx_);
                cv_.wait(lk, [this]{ return stop_ || !tasks_.empty(); });
                if (stop_ && tasks_.empty()) return;
                job = std::move(tasks_.front());
                tasks_.pop();
            }
            job();
        }
    }

    std::vector<std::thread> threads_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
    bool stop_;
};
