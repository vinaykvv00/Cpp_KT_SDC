#include <iostream>
#include <csignal>
#include <unistd.h>

bool running = true;

void stop(int sig) {
    running = false;
    std::cout << "\nGracefully shutting down...\n";
}

int main() {
    signal(SIGINT, stop);

    while (running) {
        std::cout << "Server running...\n";
        sleep(1);
    }

    std::cout << "Cleanup done. Bye!\n";
}
