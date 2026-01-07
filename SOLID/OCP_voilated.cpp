#include <iostream>
#include <string>

class AreaCalculator {
public:
    double calculateArea(const std::string& shapeType, double a, double b = 0) {
        if (shapeType == "rectangle") {
            return a * b;
        }
        else if (shapeType == "circle") {
            return 3.14159 * a * a;
        }
        else if (shapeType == "triangle") {
            return 0.5 * a * b;
        }
        else {
            std::cout << "Unknown shape!" << std::endl;
            return 0.0;
        }
    }
};

int main() {
    AreaCalculator calc;

    std::cout << "Rectangle area: " 
              << calc.calculateArea("rectangle", 10, 5) << "\n";

    std::cout << "Circle area: " 
              << calc.calculateArea("circle", 7) << "\n";

    std::cout << "Triangle area: " 
              << calc.calculateArea("triangle", 10, 6) << "\n";

    return 0;
}
