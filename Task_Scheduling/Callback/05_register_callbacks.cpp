#include <iostream>
#include <functional>
#include <vector>
using namespace std;

class EventManager {
private:
    vector<function<void(string)>> callbacks;
    
public:
    void registerCallback(function<void(string)> callback) {
        callbacks.push_back(callback);
    }

    void trigger(string message) {
        cout << "Triggering event..." << endl;
        for (auto& callback : callbacks) {
            callback(message);
        }
    }
};

int main() {
    EventManager manager;

    manager.registerCallback([](string msg) {
        cout << "Callback 1: " << msg << endl;
    });
    
    manager.registerCallback([](string msg) {
        cout << "Callback 2: Processing - " << msg << endl;
    });
    
    manager.registerCallback([](string msg) {
        cout << "Callback 3: Logging - " << msg << endl;
    });

    manager.trigger("Hello Event!");
    
    return 0;
}
