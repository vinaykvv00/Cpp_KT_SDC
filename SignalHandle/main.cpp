#include <iostream>
#include <csignal>
#include <unistd.h>

void handleSignal(int sig) {
    std::cout << "\nReceived signal: " << sig << " (Ctrl+C pressed)" << std::endl;
}

int main() {
    signal(SIGINT, handleSignal);

    std::cout << "Running program. Press Ctrl+C to trigger signal...\n";

    int count = 10;
    while (count > 0)
    {
        std::cout << "Working...\n";
        sleep(1);
        count--;
    }

    return 0;
}
