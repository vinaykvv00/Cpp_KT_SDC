#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public:
    std::string type;       
    std::string username;   
    std::string room_name;  
    std::string content;     

    Message();
    Message(const std::string& type, const std::string& username, 
            const std::string& room_name, const std::string& content = "");

    ~Message(); 

    [[nodiscard]] std::string serialize() const; 
    [[nodiscard]] static Message deserialize(const std::string& data); 
};

#endif 