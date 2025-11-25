#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <atomic>
#include <netinet/in.h>
#include <string>
#include <thread>
#include "Message.h"

class ChatClient {
public:
    ChatClient(const std::string& serverIp, int serverPort);
    ~ChatClient();

    void run(); 
    void stop();

private:
    int sockfd_{-1};
    sockaddr_in server_addr_{};
    std::string username_;
    std::string current_room_;
    std::atomic<bool> running_{true};
    std::thread receiver_thread_;

    void startReceiver();
    void receiverLoop();
    void sendMessage(const Message& msg);
    void joinRoom(const std::string& room);
    void leaveRoom();
};

#endif 
