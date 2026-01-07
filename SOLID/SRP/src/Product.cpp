#include "Product.hpp"

Product::Product(std::string name, double price) : name_(std::move(name)), price_(price) {}

std::string Product::getName() const {
    return name_;
}

double Product::getPrice() const {
    return price_;
}
