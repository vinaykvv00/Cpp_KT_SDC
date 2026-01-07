#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Shape.hpp"

class Triangle : public Shape {
private:
    double base_;
    double height_;

public:
    Triangle(double base, double height);
    double area() const override;
};

#endif
