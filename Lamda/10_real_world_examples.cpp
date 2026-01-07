// Real World Lambda Examples
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>

struct Person {
    std::string name;
    int age;
};

int main() {

    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;
    });

    std::cout << "Sorted by age:" << std::endl;
    for (const auto& p : people) {
        std::cout << p.name << " (" << p.age << ")" << std::endl;
    }

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> evens;

    std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(evens),
                 [](int n) { return n % 2 == 0; });

    std::cout << "\nEven numbers: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << std::endl;

    // Example 3: Event handlers
    auto onClick = [](int x, int y) {
        std::cout << "Clicked at (" << x << ", " << y << ")" << std::endl;
    };
    onClick(100, 200);

    // Example 4: Callback functions
    auto processData = [](const std::string& data, 
                         std::function<void(const std::string&)> callback) {
        std::cout << "Processing: " << data << std::endl;
        callback(data);
    };

    processData("Important data", [](const std::string& result) {
        std::cout << "Callback received: " << result << std::endl;
    });

    std::vector<std::string> words = {"apple", "Banana", "cherry", "Date"};
    
    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
        std::string a_lower = a;
        std::string b_lower = b;
        std::transform(a_lower.begin(), a_lower.end(), a_lower.begin(), ::tolower);
        std::transform(b_lower.begin(), b_lower.end(), b_lower.begin(), ::tolower);
        return a_lower < b_lower;
    });
    
    return 0;
}
