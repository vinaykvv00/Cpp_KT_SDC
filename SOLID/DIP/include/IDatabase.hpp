#ifndef IDATABASE_HPP
#define IDATABASE_HPP

#include <string>

class IDatabase {
public:
    virtual ~IDatabase() = default;
    
    virtual void save(const std::string& data) = 0;
};

#endif
