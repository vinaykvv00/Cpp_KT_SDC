#include <iostream>
#include "../include/ThreadPool.hpp"

int main() {
    ThreadPool pool(4);

    auto f1 = pool.submit([]{ return 42; });
    auto f2 = pool.submit([]{ long long s = 0; for (int i = 0; i < 1'000'00; ++i) s += i; return s; });
    auto f3 = pool.submit([](int a, int b){ return a + b; }, 10, 32);

    std::cout << "Result f1: " << f1.get() << std::endl;
    std::cout << "Result f2: " << f2.get() << std::endl;
    std::cout << "Result f3: " << f3.get() << std::endl;
    return 0;
}
