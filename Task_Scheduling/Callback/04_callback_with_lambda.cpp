// Callback with Lambda (Modern C++)
#include <iostream>
#include <functional>
using namespace std;

void process(int value, function<void(int)> callback) {
    cout << "Processing value: " << value << endl;
    callback(value);
}

int main() {

    process(10, [](int x) {
        cout << "Lambda callback: " << x * 2 << endl;
    });
    
    int multiplier = 5;
    process(10, [multiplier](int x) {
        cout << "Captured callback: " << x * multiplier << endl;
    });
    
    return 0;
}
