#include <iostream>
using namespace std;

void processData(int data, int (*callback)(int)) {
    cout << "Processing: " << data << endl;
    int result = callback(data);
    cout << "Result: " << result << endl;
}

int square(int x) {
    return x * x;
}

int doubleValue(int x) {
    return x * 2;
}

int main() {
    processData(5, square);       
    processData(5, doubleValue);   
    
    return 0;
}

