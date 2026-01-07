#ifndef MYSQLDATABASE_HPP
#define MYSQLDATABASE_HPP

#include "IDatabase.hpp"
#include <iostream>
#include <string>

class MySQLDatabase : public IDatabase {
public:
    MySQLDatabase() = default;

    void save(const std::string& data) override;
};

#endif
