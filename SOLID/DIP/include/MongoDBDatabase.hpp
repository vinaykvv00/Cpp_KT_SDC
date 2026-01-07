#ifndef MONGODBDATABASE_HPP
#define MONGODBDATABASE_HPP

#include "IDatabase.hpp"
#include <iostream>
#include <string>

class MongoDBDatabase : public IDatabase {
public:
    MongoDBDatabase() = default;
    ~MongoDBDatabase() override = default;

    void save(const std::string& data) override;
};

#endif
