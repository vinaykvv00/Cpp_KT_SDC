#include "Message.h"

Message::Message(int id, std::string data) : id_(id), data_(data) {}

int Message::getId() const {
    return id_;
}

std::string Message::getData() const {
    return data_;
}
