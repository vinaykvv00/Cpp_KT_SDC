#include "MongoDBDatabase.hpp"

void MongoDBDatabase::save(const std::string& data) {
    std::cout << "Executing MongoDB Function: db.users.insert({name: '"
              << data << "'})" << std::endl;
}
