#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Shape.hpp"

class Rectangle : public Shape {
private:
    double width_;
    double height_;

public:
    Rectangle(double width, double height);
    double area() const override;
};

#endif
