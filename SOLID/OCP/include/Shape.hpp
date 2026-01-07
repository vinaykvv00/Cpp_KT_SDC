#ifndef SHAPE_HPP
#define SHAPE_HPP

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

#endif
