#ifndef EXTENDED_STRING_SDK_H
#define EXTENDED_STRING_SDK_H

#include <string>
#include <vector>

namespace ExtendedString {

    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
}

extern "C" {
    const char* extended_string_toUpper(const char* str);
    const char* extended_string_toLower(const char* str);
    const char* extended_string_trim(const char* str);
}

#endif