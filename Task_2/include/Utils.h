#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    Utils() = default; 
    ~Utils() = default; 

    [[nodiscard]] static std::vector<std::string> split(const std::string& str, char delimiter); 
    [[nodiscard]] static std::string join(const std::vector<std::string>& vec, char delimiter);
};

#endif 
