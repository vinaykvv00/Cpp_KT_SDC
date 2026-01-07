#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

void setValue(std::promise<int> prom)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    prom.set_value(100);

    //do some othrer tasks
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Value set in promise." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Promise thread ending." << std::endl;
}

int main()
{
    std::promise<int> promise_value;
    std::future<int> fut = promise_value.get_future();

    std::cout << "Setting value in promise thread..." << std::endl;
    std::thread promiseThread(setValue, std::move(promise_value));

    std::cout << "Waiting for value from promise..." << std::endl;
    std::cout << "value receving from promise is: " << fut.get() << std::endl;

    std::cout << "Main thread ending." << std::endl;
    promiseThread.join();
    return 0;
}
