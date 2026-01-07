#include "extended_string_sdk.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace ExtendedString {

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string trim(const std::string& str) {

        const char* whitespace = " \t\n\r\f\v";
        
        size_t start = str.find_first_not_of(whitespace);
        
        if (start == std::string::npos) {
            return "";
        }
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::string current = "";

        for (char c : str) {
            if (c == delimiter) {
                result.push_back(current);
                current = ""; 
            } else {
                current += c;
            }
        }
        result.push_back(current);
        
        return result;
    }

}

extern "C" {
    const char* extended_string_toUpper(const char* str) {
        static thread_local std::string result;
        result = ExtendedString::toUpper(str);
        return result.c_str();
    }

    const char* extended_string_toLower(const char* str) {
        static thread_local std::string result;
        result = ExtendedString::toLower(str);
        return result.c_str();
    }

    const char* extended_string_trim(const char* str) {
        static thread_local std::string result;
        result = ExtendedString::trim(str);
        return result.c_str();
    }
}
