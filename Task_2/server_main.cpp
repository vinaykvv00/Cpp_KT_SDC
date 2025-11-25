#include "ChatServer.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    try {
        ChatServer server(port);
        std::thread control([&server]() {
            std::string line;
            while (std::getline(std::cin, line)) {
                if (line == "/quit" || line == "/shutdown") {
                    server.stop();
                    break;
                }
            }
        });
        control.detach(); // Do not block shutdown if stdin waits
        server.run();
    } catch (const std::exception& ex) {
        std::cerr << "Server error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
