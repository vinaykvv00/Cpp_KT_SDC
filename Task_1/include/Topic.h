#pragma once
#include "Message.h"
#include <vector>
#include <mutex>
#include <deque>
#include <condition_variable>

class Topic {
public:
    Topic(std::string name);
    ~Topic();

    void publish(const Message& msg);
    bool consume(int consumerId, Message& msg);
    void registerConsumer(int consumerId);
    void shutdown();
    
    std::string getName() const;
    
private:
    std::string name_;
    std::deque<Message> messages_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool isShutdown_;

    std::vector<int> consumers_;
    std::vector<size_t> consumerOffsets_;
};
