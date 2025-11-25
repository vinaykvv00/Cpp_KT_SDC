#include "Message.h"
#include "Utils.h"

Message::Message()  : type(""), username(""), room_name(""), content("") {}

Message::Message(const std::string& type, const std::string& username,
                 const std::string& room_name, const std::string& content)
    : type(type), username(username), room_name(room_name), content(content) {}

Message::~Message() = default; 

std::string Message::serialize() const {
    if (type == "JOIN" || type == "LEAVE") {
        return type + "|" + username + "|" + room_name;
    }
    else if (type == "CHAT") {
        return type + "|" + username + "|" + room_name + "|" + content;
    }
    return "";
}

Message Message::deserialize(const std::string& data) {
    std::vector<std::string> parts = Utils::split(data, '|');
    
    Message msg;
    if (parts.size() < 2) {
        return msg;
    }

    msg.type = parts[0];
    msg.username = parts[1];
    
    if (parts[0] == "CHAT" && parts.size() >= 4) {
        msg.room_name = parts[2];
        msg.content = parts[3];
    }
    else if (parts.size() >= 3) {
        msg.room_name = parts[2];
    }
    
    return msg;
}
