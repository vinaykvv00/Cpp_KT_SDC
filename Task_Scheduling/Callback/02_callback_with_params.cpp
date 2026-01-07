#include <iostream>
using namespace std;

void processData(int value) {
    cout << "Processing: " << value << endl;
    cout << "Result: " << value * 2 << endl;
}

void displayData(int value) {
    cout << "Displaying: " << value << endl;
}

void handleData(int data, void (*callback)(int)) {
    cout << "Handling data..." << endl;
    callback(data);  
}

int main() {
    handleData(10, processData);
    cout << "---" << endl;
    handleData(25, displayData);
    
    return 0;
}
