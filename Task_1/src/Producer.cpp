#include "Producer.h"
#include "Utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

Producer::Producer(int id, std::string name) : id_(id), name_(name), running_(false) {}

Producer::~Producer() {
    stop();
}

void Producer::start(Topic& topic, int messageCount) {
    running_ = true;
    auto topicPtr = std::shared_ptr<Topic>(&topic, [](Topic*){});
    thread_ = std::thread(&Producer::produceLoop, this, topicPtr, messageCount);
}

void Producer::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

std::string Producer::getName() const {
    return name_;
}

void Producer::produceLoop(std::shared_ptr<Topic> topic, int count) {
    for (int i = 0; i < count && running_; i++) {
        Message msg(id_ * 1000 + i, name_ + "_msg_" + std::to_string(i));
        topic->publish(msg);
        
        std::stringstream ss;
        ss << "[Producer " << name_ << "] Published: " 
           << msg.getId() << " - " << msg.getData();
        Utils::print(ss.str());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
