#include "Room.h"
#include <algorithm>

UserInfo::UserInfo(const std::string& user, const std::string& ip_addr, int port_num)
    : username(user), ip(ip_addr), port(port_num) {}

bool UserInfo::operator==(const UserInfo& other) const {
    return username == other.username;
}

Room::Room() : room_name_("") {}

Room::Room(const std::string& name) : room_name_(name) {}

Room::~Room() = default;

void Room::addUser(const std::string& username, const std::string& ip, int port) {
    std::lock_guard<std::mutex> lock(mtx_);
    
    for (const auto& member : members_) {
        if (member.username == username) {
            return; 
        }
    }
    
    members_.emplace_back(username, ip, port);
}

void Room::removeUser(const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx_);
    
    members_.erase(
        std::remove_if(members_.begin(), members_.end(),
            [&username](const UserInfo& user) {
                return user.username == username;
            }),
        members_.end()
    );
}

std::vector<UserInfo> Room::getMembers() {
    std::lock_guard<std::mutex> lock(mtx_);
    return members_; 
}

std::string Room::getName() const {
    return room_name_;
}
