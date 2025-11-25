#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!\n";
    }
    else if (pid == 0) {
        std::cout << "Child Process. PID = " << getpid() << "\n";
    }
    else {
        std::cout << "Parent Process. PID = " << getpid()
                  << ", Child PID = " << pid << "\n";
    }
    return 0;
}
