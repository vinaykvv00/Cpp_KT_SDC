#include "Topic.h"
#include <algorithm>

Topic::Topic(std::string name) : name_(name), isShutdown_(false) {}

Topic::~Topic() {
    shutdown();
}

void Topic::publish(const Message& msg) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (isShutdown_) { return; }
    
    messages_.push_back(msg);
    cv_.notify_all(); 
}

bool Topic::consume(int consumerId, Message& msg) {
    std::unique_lock<std::mutex> lock(mtx_);
    
    auto it = std::find(consumers_.begin(), consumers_.end(), consumerId);
    if (it == consumers_.end()) {
        return false; 
    }
    
    size_t index = it - consumers_.begin();
    
    cv_.wait(lock, [this, index]() {
        return consumerOffsets_[index] < messages_.size() || isShutdown_;
    });
    
    if (isShutdown_ && consumerOffsets_[index] >= messages_.size()) {
        return false;
    }
    
    msg = messages_[consumerOffsets_[index]];
    consumerOffsets_[index]++;
    
    return true;
}

void Topic::registerConsumer(int consumerId) {
    std::lock_guard<std::mutex> lock(mtx_);
    consumers_.push_back(consumerId);
    consumerOffsets_.push_back(0); 
}

void Topic::shutdown() {
    std::lock_guard<std::mutex> lock(mtx_);
    isShutdown_ = true;
    cv_.notify_all();
}

std::string Topic::getName() const {
    return name_;
}
