#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    auto lambda1 = [x]() {
        std::cout << "Captured x = " << x << std::endl;
    };
    lambda1();

    auto lambda2 = [=]() {
        std::cout << "x = " << x << ", y = " << y << std::endl;
    };
    lambda2();

    auto lambda3 = [x, y]() {
        int sum = x + y;
        std::cout << "Sum = " << sum << std::endl;
    };
    lambda3();

    x = 100;
    y = 200;
    std::cout << "\nAfter changing x and y:" << std::endl;
    lambda1(); 
    lambda2(); 

    return 0;
}
