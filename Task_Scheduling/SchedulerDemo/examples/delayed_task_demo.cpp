#include <iostream>
#include <chrono>
#include "../src/delayed_task.cpp"

int main() {
    using namespace std::chrono_literals;
    std::cout << "Scheduling tasks..." << std::endl;
    schedule_after(500ms, []{ std::cout << "A after 500ms" << std::endl; });
    schedule_after(1000ms, []{ std::cout << "B after 1000ms" << std::endl; });
    std::this_thread::sleep_for(1200ms);
    return 0;
}
