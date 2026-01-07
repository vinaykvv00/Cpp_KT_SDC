#include "Rectangle.hpp"

Rectangle::Rectangle(int w, int h) : width_(w), height_(h) {}

int Rectangle::area() const {
    return width_ * height_;
}
