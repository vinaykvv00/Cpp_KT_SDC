#include <iostream>
#include <algorithm>
#include <vector>

// [capture] (params) { body }

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    for(auto n : numbers) {
        std::cout << "Elements: " << n << std::endl;
    }

    std::cout << "Using lambda to print elements:" << std::endl;
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << "Lambda Element: " << n << std::endl;
    });
}