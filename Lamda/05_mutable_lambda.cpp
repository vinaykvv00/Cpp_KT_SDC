#include <iostream>

int main() {
    int x = 10;

    auto lambda1 = [x]() {
        std::cout << "x = " << x << std::endl;
        // x = 100; // Error: cannot modify
    };
    lambda1();

    auto lambda2 = [x]() mutable {
        x = 100; // OK: modifies the copy
        std::cout << "Inside lambda: x = " << x << std::endl;
    };
    lambda2();
    std::cout << "Outside lambda: x = " << x << std::endl;

    auto counter = [count = 0]() mutable {
        count++;
        std::cout << "Count = " << count << std::endl;
        return count;
    };
    counter(); 
    counter(); 
    counter(); 

    return 0;
}
