// Lambda Return Types
#include <iostream>
#include <string>

int main() {
    // Auto return type deduction
    auto add = [](int a, int b) {
        return a + b; // Compiler deduces return type as int
    };
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;

    // Explicit return type
    auto divide = [](double a, double b) -> double {
        return a / b;
    };
    std::cout << "10 / 3 = " << divide(10.0, 3.0) << std::endl;

    auto compare = [](int a, int b) -> std::string {
        if (a > b) return "greater";
        else if (a < b) return "less";
        else return "equal";
    };
    std::cout << "5 is " << compare(5, 3) << " than 3" << std::endl;

    auto print = [](const std::string& msg) -> void {
        std::cout << msg << std::endl;
    };
    print("Hello from lambda");

    return 0;
}
