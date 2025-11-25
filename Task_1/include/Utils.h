#pragma once
#include <iostream>
#include <mutex>
#include <string>

class Utils {
public:
    static void print(const std::string& msg);
private:
    static std::mutex printMutex_;
};
