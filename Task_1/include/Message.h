#pragma once
#include <string>

class Message {
public:
    Message(int id, std::string data);
    ~Message() = default;
    
    int getId() const;
    std::string getData() const;
    
private:
    int id_;
    std::string data_;
};
