#ifndef USERSERVICE_HPP
#define USERSERVICE_HPP

#include "IDatabase.hpp"
#include <memory>
#include <string>

class UserService {
private:
    std::shared_ptr<IDatabase> db_; 

public:
    explicit UserService(std::shared_ptr<IDatabase> db);
    ~UserService() = default;

    void storeUser(const std::string& username);
};

#endif
