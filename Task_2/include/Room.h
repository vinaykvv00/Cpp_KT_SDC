#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <mutex>

struct UserInfo {
    std::string username;
    std::string ip;
    int port;

    UserInfo(const std::string& user, const std::string& ip_addr, int port_num);
    
    bool operator==(const UserInfo& other) const;
};

class Room {
private:
    std::string room_name_;
    std::vector<UserInfo> members_;
    std::mutex mtx_;

public:
    Room();
    explicit Room(const std::string& name);

    ~Room(); 

    void addUser(const std::string& username, const std::string& ip, int port);
    void removeUser(const std::string& username);
    [[nodiscard]] std::vector<UserInfo> getMembers(); 
    [[nodiscard]] std::string getName() const; 
};

#endif 
