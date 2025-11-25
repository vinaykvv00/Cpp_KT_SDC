#include <iostream>
#include <unistd.h>

int main() {
    fork(); 
    fork();  

    std::cout << "PID: " << getpid() << "\n";
    return 0;
}
