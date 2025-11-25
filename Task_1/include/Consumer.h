#pragma once
#include "Topic.h"
#include <thread>
#include <string>
#include <memory>

class Consumer {
public:
    Consumer(int id, std::string name);
    ~Consumer();
    
    void start(Topic& topic, int messageCount);
    void stop();
    std::string getName() const;
    int getMessagesReceived() const;
    
private:
    void consumeLoop(std::shared_ptr<Topic> topic, int count);
    
    int id_;
    std::string name_;
    std::thread thread_;
    bool running_;
    int messagesReceived_;
};
