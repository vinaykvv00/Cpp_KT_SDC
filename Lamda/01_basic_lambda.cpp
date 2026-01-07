#include <iostream>

int main() {

    auto hello = []() {
        std::cout << "Hello from lambda" << std::endl;
    };
    hello();

    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;

    auto multiply = [](int a, int b) -> int {
        return a * b;
    };
    std::cout << "4 * 7 = " << multiply(4, 7) << std::endl;

    int result = [](int x) { return x * x; }(10);
    std::cout << "10 squared = " << result << std::endl;

    return 0;
}
