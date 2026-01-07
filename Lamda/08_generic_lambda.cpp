#include <iostream>
#include <string>

int main() {
    
    auto print = [](auto value) {
        std::cout << "Value: " << value << std::endl;
    };
    
    print(42);          
    print(3.14);         
    print("Hello");      
    print(std::string("World"));

    auto add = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;         
    std::cout << "2.5 + 1.5 = " << add(2.5, 1.5) << std::endl;   
    
    std::string s1 = "Hello";
    std::string s2 = "World";
    std::cout << add(s1, s2) << std::endl; 

    auto compare = [](auto a, auto b) {
        return a > b;
    };
    
    std::cout << "10 > 5: " << compare(10, 5) << std::endl;
    std::cout << "3.14 > 2.71: " << compare(3.14, 2.71) << std::endl;

    return 0;
}
