#include "ChatClient.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }
    std::string server_ip = argv[1];
    int server_port = std::atoi(argv[2]);
    try {
        ChatClient client(server_ip, server_port);
        client.run();
    } catch (const std::exception& ex) {
        std::cerr << "Client error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
