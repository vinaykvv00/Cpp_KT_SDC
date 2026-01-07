#pragma once
#include "Shape.hpp"

class Rectangle : public Shape {
public:
    Rectangle(int w, int h);

    int area() const override;

private:
    int width_;
    int height_;
};
