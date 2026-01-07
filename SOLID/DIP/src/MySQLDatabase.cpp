#include "MySQLDatabase.hpp"

void MySQLDatabase::save(const std::string& data) {
    std::cout << "Executing SQL Query: INSERT INTO users VALUES('"
              << data << "');" << std::endl;
}
