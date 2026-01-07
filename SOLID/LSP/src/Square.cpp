#include "Square.hpp"

Square::Square(int side) : side_(side) {}

int Square::area() const {
    return side_ * side_;
}
