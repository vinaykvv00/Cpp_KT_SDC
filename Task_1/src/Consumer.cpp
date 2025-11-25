#include "Consumer.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

Consumer::Consumer(int id, std::string name) : id_(id), name_(name), running_(false), messagesReceived_(0) {}

Consumer::~Consumer() {
    stop();
}

void Consumer::start(Topic& topic, int messageCount) {
    topic.registerConsumer(id_);
    running_ = true;
    auto topicPtr = std::shared_ptr<Topic>(&topic, [](Topic*){});
    thread_ = std::thread(&Consumer::consumeLoop, this, topicPtr, messageCount);
}

void Consumer::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

std::string Consumer::getName() const {
    return name_;
}

int Consumer::getMessagesReceived() const {
    return messagesReceived_;
}

void Consumer::consumeLoop(std::shared_ptr<Topic> topic, int count) {
    for (int i = 0; i < count && running_; i++) {
        Message msg(0, "");
        if (topic->consume(id_, msg)) {
            messagesReceived_++;
            
            std::stringstream ss;
            ss << "[Consumer " << name_ << "] Received: " 
               << msg.getId() << " - " << msg.getData();
            Utils::print(ss.str());
        } else {
            break; 
        }
    }
}
