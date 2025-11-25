#pragma once
#include "Topic.h"
#include <thread>
#include <string>
#include <memory>

class Producer {
public:
    Producer(int id, std::string name);
    ~Producer();
    
    void start(Topic& topic, int messageCount);
    void stop();
    std::string getName() const;
    
private:
    void produceLoop(std::shared_ptr<Topic> topic, int count);
    
    int id_;
    std::string name_;
    std::thread thread_;
    bool running_;
};
