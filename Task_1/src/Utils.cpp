#include "Utils.h"

std::mutex Utils::printMutex_;

void Utils::print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(printMutex_);
    std::cout << msg << std::endl;
}
