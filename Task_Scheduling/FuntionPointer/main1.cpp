#include <iostream>
using namespace std;

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int (*operation)(int, int);
    
    operation = add;
    cout << "5 + 3 = " << operation(5, 3) << endl;
    
    operation = multiply;
    cout << "5 * 3 = " << operation(5, 3) << endl;
    
    return 0;
}
