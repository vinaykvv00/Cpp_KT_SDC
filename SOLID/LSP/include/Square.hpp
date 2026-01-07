#pragma once
#include "Shape.hpp"

class Square : public Shape {
public:
    explicit Square(int side);

    int area() const override;

private:
    int side_;
};
