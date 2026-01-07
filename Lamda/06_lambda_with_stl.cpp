#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7};

    std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a < b;
    });
    std::cout << "Sorted: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    auto it = std::find_if(numbers.begin(), numbers.end(), [](int n) {
        return n > 5;
    });
    if (it != numbers.end()) {
        std::cout << "First number > 5: " << *it << std::endl;
    }

    int count = std::count_if(numbers.begin(), numbers.end(), [](int n) {
        return n % 2 == 0;
    });
    std::cout << "Even numbers count: " << count << std::endl;

    std::vector<int> squared(numbers.size());
    std::transform(numbers.begin(), numbers.end(), squared.begin(), [](int n) {
        return n * n;
    });
    std::cout << "Squared: ";
    for (int n : squared) std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "Doubled: ";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << (n * 2) << " ";
    });
    std::cout << std::endl;

    return 0;
}
