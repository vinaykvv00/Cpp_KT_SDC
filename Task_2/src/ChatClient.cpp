#include "ChatClient.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

ChatClient::ChatClient(const std::string& serverIp, int serverPort) {
    sockfd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    std::memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIp.c_str(), &server_addr_.sin_addr) <= 0) {
        ::close(sockfd_);
        throw std::runtime_error("Invalid server IP address");
    }
}

ChatClient::~ChatClient() {
    stop();
    if (receiver_thread_.joinable()) {
        receiver_thread_.join();
    }
}

void ChatClient::startReceiver() {
    receiver_thread_ = std::thread(&ChatClient::receiverLoop, this);
}

void ChatClient::receiverLoop() {
    char buffer[1024];
    while (running_) {
        sockaddr_in from_addr{};
        socklen_t addr_len = sizeof(from_addr);
        int recv_len = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0,
                                reinterpret_cast<struct sockaddr*>(&from_addr), &addr_len);
        if (recv_len < 0) {
            if (running_) {
                std::cerr << "recvfrom error" << std::endl;
            }
            break;
        }
        buffer[recv_len] = '\0';
        Message msg = Message::deserialize(std::string(buffer));
        if (msg.type == "CHAT") {
            std::cout << "\n[" << msg.username << "]: " << msg.content << std::endl;
            std::cout << "> " << std::flush;
        }
    }
}

void ChatClient::stop() {
    if (!running_) return;
    running_ = false;
    if (sockfd_ >= 0) {
        ::close(sockfd_); 
        sockfd_ = -1;
    }
}

void ChatClient::sendMessage(const Message& msg) {
    std::string data = msg.serialize();
    sendto(sockfd_, data.c_str(), data.length(), 0,
           reinterpret_cast<struct sockaddr*>(&server_addr_), sizeof(server_addr_));
}

void ChatClient::joinRoom(const std::string& room) {
    current_room_ = room;
    Message msg("JOIN", username_, current_room_);
    sendMessage(msg);
    std::cout << "✓ Joined room '" << current_room_ << "'" << std::endl;
}

void ChatClient::leaveRoom() {
    if (current_room_.empty()) {
        std::cout << "Not in any room" << std::endl;
        return;
    }
    Message msg("LEAVE", username_, current_room_);
    sendMessage(msg);
    std::cout << "✓ Left room '" << current_room_ << "'" << std::endl;
    current_room_.clear();
}

void ChatClient::run() {
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "    UDP CHAT CLIENT\n";
    std::cout << "    Connected to server: " << "(set)" << std::endl; 
    std::cout << "═══════════════════════════════════════\n";

    std::cout << "\nEnter username: ";
    std::getline(std::cin, username_);
    std::cout << "Welcome, " << username_ << "!" << std::endl;

    std::cout << "\nCommands:\n";
    std::cout << "  /join <room>    - Join a room\n";
    std::cout << "  /leave          - Leave current room\n";
    std::cout << "  /quit           - Exit application\n\n";

    startReceiver();

    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }
        if (input.rfind("/join ", 0) == 0) {
            if (input.length() <= 6) {
                std::cout << "Usage: /join <room_name>" << std::endl;
                continue;
            }
            joinRoom(input.substr(6));
        } else if (input == "/leave") {
            leaveRoom();
        } else if (input == "/quit") {
            if (!current_room_.empty()) {
                leaveRoom();
            }
            std::cout << "Goodbye!" << std::endl;
            stop(); 
            break;
        } else {
            if (current_room_.empty()) {
                std::cout << "Join a room first with /join <room>" << std::endl;
                continue;
            }
            Message msg("CHAT", username_, current_room_, input);
            sendMessage(msg);
        }
    }
}
