#include <iostream>
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Triangle.hpp"
#include "AreaCalculator.hpp"

int main() {
    Rectangle rect(10, 5);
    Circle circle(7);
    Triangle triangle(10, 6);

    AreaCalculator calc;

    std::cout << "Rectangle area: " << calc.calculate(rect) << "\n";
    std::cout << "Circle area: " << calc.calculate(circle) << "\n";
    std::cout << "Triangle area: " << calc.calculate(triangle) << "\n";

    return 0;
}
