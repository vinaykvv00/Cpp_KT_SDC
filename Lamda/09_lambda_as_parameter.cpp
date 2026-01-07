#include <iostream>
#include <vector>
#include <functional>

void processNumbers(const std::vector<int>& nums, std::function<void(int)> func) {
    for (int n : nums) {
        func(n);
    }
}

std::function<int(int)> makeAdder(int x) {
    return [x](int y) { return x + y; };
}

template<typename Func>
void applyOperation(int a, int b, Func operation) {
    std::cout << "Result: " << operation(a, b) << std::endl;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "Numbers doubled: ";
    processNumbers(numbers, [](int n) {
        std::cout << (n * 2) << " ";
    });
    std::cout << std::endl;

    auto add5 = makeAdder(5);
    std::cout << "10 + 5 = " << add5(10) << std::endl;

    applyOperation(10, 3, [](int a, int b) { return a + b; });
    applyOperation(10, 3, [](int a, int b) { return a * b; });
    applyOperation(10, 3, [](int a, int b) { return a - b; });

    std::function<int(int, int)> add = [](int a, int b) { return a + b; };
    std::function<int(int, int)> multiply = [](int a, int b) { return a * b; };
    
    std::cout << "Add: " << add(5, 3) << std::endl;
    std::cout << "Multiply: " << multiply(5, 3) << std::endl;

    return 0;
}
