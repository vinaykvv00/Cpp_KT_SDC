#include <iostream>
#include "Rectangle.hpp"
#include "Square.hpp"

void printArea(Shape* s) {
    std::cout << "Area = " << s->area() << std::endl;
}

int main() {
    Shape* r = new Rectangle(4, 5);
    Shape* s = new Square(5);

    printArea(r);
    printArea(s);

    delete r;
    delete s;

    return 0;
}
