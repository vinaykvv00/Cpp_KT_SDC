#include "UserService.hpp"

UserService::UserService(std::shared_ptr<IDatabase> db) : db_(std::move(db)) {}

void UserService::storeUser(const std::string& username) {
    db_->save(username);
}
