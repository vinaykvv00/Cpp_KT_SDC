// Lambda Capture by Reference
#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    auto lambda1 = [&x]() {
        x = 100; 
        std::cout << "Modified x = " << x << std::endl;
    };
    lambda1();
    std::cout << "x after lambda = " << x << std::endl;

    auto lambda2 = [&]() {
        x += 50;
        y += 100;
        std::cout << "x = " << x << ", y = " << y << std::endl;
    };
    lambda2();
    std::cout << "After lambda2: x = " << x << ", y = " << y << std::endl;

    auto lambda3 = [&x, &y]() {
        x *= 2;
        y *= 2;
    };
    lambda3();
    std::cout << "After lambda3: x = " << x << ", y = " << y << std::endl;

    return 0;
}
