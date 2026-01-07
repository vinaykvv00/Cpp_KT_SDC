#include <iostream>
using namespace std;

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void calculate(int x, int y, int (*operation)(int, int)) {
    int result = operation(x, y);
    cout << "Result: " << result << endl;
}

int main() {
    calculate(5, 3, add);       
    calculate(5, 3, multiply);  
    
    return 0;
}
