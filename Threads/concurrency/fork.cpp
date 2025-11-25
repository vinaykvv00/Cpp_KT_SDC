#include <iostream>
#include <unistd.h>   
#include <sys/types.h>

int main() {
    std::cout << "Before fork, PID: " << getpid() << std::endl;

    pid_t pid = fork();  

    if (pid < 0) {
        std::cerr << "Fork failed!\n";
        return 1;
    } else if (pid == 0) {
        std::cout << "Child Process: PID = " << getpid() << ", Parent PID = " << getppid() << std::endl;
    } else {
        std::cout << "Parent Process: PID = " << getpid() << ", Child PID = " << pid << std::endl;
    }

    std::cout << "Both parent and child reach here.\n";
    return 0;
}
