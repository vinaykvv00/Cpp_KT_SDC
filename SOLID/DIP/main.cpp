#include "UserService.hpp"
#include "MySQLDatabase.hpp"
#include "MongoDBDatabase.hpp"
#include <memory>

int main() {
    std::shared_ptr<IDatabase> mysql = std::make_shared<MySQLDatabase>();
    std::shared_ptr<IDatabase> mongo = std::make_shared<MongoDBDatabase>();

    UserService service1(mysql);
    service1.storeUser("Aditya");

    UserService service2(mongo);
    service2.storeUser("Rohit");

    return 0;
}
