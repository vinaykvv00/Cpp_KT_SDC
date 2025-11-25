#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    std::cout << "Program started. PID = " << getpid() << "\n\n";

    pid_t pid = fork();  

    if (pid < 0) {
        std::cerr << "Fork failed!\n";
        return 1;
    }

    if (pid == 0) {
        std::cout << "Child: I am the worker. PID = " << getpid() << "\n";
        std::cout << "Child: Starting heavy task...\n";

        for (int i = 1; i <= 5; i++) {
            std::cout << "Child: Working... step " << i << "\n";
            sleep(1);
        }

        std::cout << "Child: Finished my work!\n\n";
        return 0;
    }

    if (pid > 0) {
        std::cout << "Parent: I created a child. Child PID = " << pid << "\n";
        std::cout << "Parent: While child works, I will do something else...\n";

        for (int i = 1; i <= 3; i++) {
            std::cout << "Parent: Checking status... " << i << "\n";
            sleep(1);
        }

        wait(nullptr);

        std::cout << "Parent: Child finished. Parent exiting.\n\n";
    }

    return 0;
}
