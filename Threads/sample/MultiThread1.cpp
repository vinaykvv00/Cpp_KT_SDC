#include <iostream>
#include <thread>
#include <bits/stdc++.h>

using namespace std;

void printHello() {
    cout << "Hello from printthread!: this thread id"<<" "<< this_thread::get_id() <<endl;
}
int main() {
    auto task = []() {
        cout << "Hello from thread!: this thread id"<<" "<< this_thread::get_id() <<endl;
    };

    thread t1(task);
    thread t2(printHello); 
    t1.detach(); 
    t2.join();

    cout << "Thread has finished execution." <<endl;
    return 0;
}