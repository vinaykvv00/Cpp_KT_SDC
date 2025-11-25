#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <atomic>
#include <netinet/in.h>
#include "Room.h"
#include "Message.h"

class ChatServer {
public:
    explicit ChatServer(int port);
    ~ChatServer();

    void run(); 
    void stop(); 

private:
    int port_{};
    int sockfd_{-1};
    std::map<std::string, std::shared_ptr<Room>> rooms_;
    std::mutex rooms_mutex_;
    std::atomic<bool> running_{true};

    void handleJoin(const Message& msg, const sockaddr_in& client_addr);
    void handleChat(const Message& msg, const sockaddr_in& sender_addr);
    void handleLeave(const Message& msg, const sockaddr_in& client_addr);
};

#endif 
