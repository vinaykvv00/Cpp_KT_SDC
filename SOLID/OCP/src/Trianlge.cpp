#include "Triangle.hpp"

Triangle::Triangle(double base, double height) : base_(base), height_(height) {}

double Triangle::area() const {
    return 0.5 * base_ * height_;
}
