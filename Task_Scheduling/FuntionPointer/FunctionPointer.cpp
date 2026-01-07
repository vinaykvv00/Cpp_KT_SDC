#include <iostream>
#include <functional>

typedef int (*PFnIntegrOperation)(int, int);
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main()
{
   // int (*op)(int, int); 
   // PFnIntegrOperation op;
    std::function<int(int, int)> op;
    std::cout << "Function Pointer Demo: 1 for add , 2 for multiply\n";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        op = &add;
    } else {
        op = &multiply;
    }
    int result = op(10, 20);
    std::cout << "Result: " << result << std::endl;
    return 0;
}