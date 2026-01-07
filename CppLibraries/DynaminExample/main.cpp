#include <extended_string_sdk.h>
#include <iostream>

int main() {
    std::string text = "hello world";
    std::cout << ExtendedString::toUpper(text) << std::endl;
    return 0;
}