#include <extended_string_sdk.h>
#include <iostream>

int main() {
    std::string text = "hello world";
    std::cout << "Original: " << text << std::endl;
    std::cout << "Upper: " << ExtendedString::toUpper(text) << std::endl;
    std::cout << "Lower: " << ExtendedString::toLower(text) << std::endl;
    
    auto words = ExtendedString::split(text, ' ');
    std::cout << "Split: ";
    for (const auto& word : words) {
        std::cout << "[" << word << "] ";
    }
    std::cout << std::endl;
    
    return 0;
}