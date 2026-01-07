#include <iostream>
using namespace std;

void greet() {
    cout << "Hello from callback!" << endl;
}

void sayGoodbye() {
    cout << "Goodbye from callback!" << endl;
}

void executeCallback(void (*callback)()) {
    cout << "Before callback" << endl;
    callback(); 
    cout << "After callback" << endl;
}

int main() {
    
    executeCallback(greet);
    cout << "---" << endl;
    executeCallback(sayGoodbye);
    
    return 0;
}
