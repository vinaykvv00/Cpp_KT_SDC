#include <dlfcn.h>
#include <iostream>

typedef const char* (*ToUpperFunc)(const char*);
typedef const char* (*ToLowerFunc)(const char*);
typedef const char* (*TrimFunc)(const char*);

int main() {
    
    void* handle = dlopen("libextended_string_sdk.so.1", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error: " << dlerror() << std::endl;
        return 1;
    }

    ToUpperFunc toUpper = (ToUpperFunc) dlsym(handle, "extended_string_toUpper");
    ToLowerFunc toLower = (ToLowerFunc) dlsym(handle, "extended_string_toLower");
    TrimFunc trim = (TrimFunc) dlsym(handle, "extended_string_trim");

    if (!toUpper || !toLower || !trim) {
        std::cerr << "Error: Failed to load functions" << std::endl;
        dlclose(handle);
        return 1;
    }

    std::cout << "Original: hello world" << std::endl;
    std::cout << "Upper: " << toUpper("hello world") << std::endl;
    std::cout << "Lower: " << toLower("HELLO WORLD") << std::endl;
    std::cout << "Trim: " << trim("  spaces  ") << std::endl;

    dlclose(handle);
    return 0;
}
