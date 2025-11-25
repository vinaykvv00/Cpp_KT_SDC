#include <iostream>
#include <unistd.h>

int main() {
    for (int i = 1; i <= 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            std::cout << "Child " << i << " running. PID = " << getpid() << "\n";
            return 0; 
        }
    }

    std::cout << "Parent process. PID = " << getpid() << "\n";
    return 0;
}
