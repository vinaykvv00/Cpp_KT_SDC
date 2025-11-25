#include "ChatServer.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

ChatServer::ChatServer(int port) : port_(port) {
    sockfd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in server_addr{};
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd_, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        ::close(sockfd_);
        throw std::runtime_error("Failed to bind socket");
    }

    std::cout << "═══════════════════════════════════════\n";
    std::cout << "    UDP CHAT SERVER STARTED\n";
    std::cout << "    Listening on port " << port_ << "\n";
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "\n[SERVER] Waiting for users to connect...\n" << std::endl;
}

ChatServer::~ChatServer() {
    if (sockfd_ >= 0) {
        ::close(sockfd_);
    }
}

void ChatServer::handleJoin(const Message& msg, const sockaddr_in& client_addr) {
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    std::string ip = inet_ntoa(client_addr.sin_addr);
    int port = ntohs(client_addr.sin_port);

    if (rooms_.find(msg.room_name) == rooms_.end()) {
        rooms_[msg.room_name] = std::make_shared<Room>(msg.room_name);
    }

    rooms_[msg.room_name]->addUser(msg.username, ip, port);
    std::cout << "[" << msg.username << "] joined room '" << msg.room_name << "'" << std::endl;
}

void ChatServer::handleChat(const Message& msg, const sockaddr_in& sender_addr) {
    (void)sender_addr; 
    auto it = rooms_.find(msg.room_name);
    if (it == rooms_.end()) {
        return;
    }

    std::vector<UserInfo> members = it->second->getMembers();
    Message outMsg("CHAT", msg.username, "", msg.content);
    std::string data = outMsg.serialize();

    for (const UserInfo& user : members) {
        sockaddr_in user_addr{};
        user_addr.sin_family = AF_INET;
        user_addr.sin_port = htons(user.port);
        inet_pton(AF_INET, user.ip.c_str(), &user_addr.sin_addr);

        sendto(sockfd_, data.c_str(), data.length(), 0,
               reinterpret_cast<struct sockaddr*>(&user_addr), sizeof(user_addr));
    }

    std::cout << "[" << msg.room_name << "] " << msg.username << ": " << msg.content << std::endl;
}

void ChatServer::handleLeave(const Message& msg, const sockaddr_in& client_addr) {
    (void)client_addr; 
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    auto it = rooms_.find(msg.room_name);
    if (it != rooms_.end()) {
        it->second->removeUser(msg.username);
        std::cout << "[" << msg.username << "] left room '" << msg.room_name << "'" << std::endl;
    }
}

void ChatServer::run() {
    while (running_) {
        char buffer[1024];
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int recv_len = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0,
                                reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);
        if (recv_len < 0) {
            std::cerr << "recvfrom error" << std::endl;
            continue;
        }
        buffer[recv_len] = '\0';

        Message msg = Message::deserialize(std::string(buffer));
        if (msg.type == "JOIN") {
            handleJoin(msg, client_addr);
        } else if (msg.type == "CHAT") {
            handleChat(msg, client_addr);
        } else if (msg.type == "LEAVE") {
            handleLeave(msg, client_addr);
        }
    }
}

void ChatServer::stop() {
    if (!running_) return;
    running_ = false;
    if (sockfd_ >= 0) {
        ::close(sockfd_); 
        sockfd_ = -1;
    }
    std::cout << "[SERVER] Shutdown initiated." << std::endl;
}
