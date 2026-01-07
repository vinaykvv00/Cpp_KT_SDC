#include <chrono>
#include <thread>
#include <functional>

inline void schedule_after(std::chrono::milliseconds delay, std::function<void()> fn) {
    std::thread([delay, fn]{
        std::this_thread::sleep_for(delay);
        fn();
    }).detach();
}
