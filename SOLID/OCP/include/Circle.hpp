#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "Shape.hpp"

class Circle : public Shape {
private:
    double radius_;

public:
    Circle(double radius);
    double area() const override;
};

#endif
