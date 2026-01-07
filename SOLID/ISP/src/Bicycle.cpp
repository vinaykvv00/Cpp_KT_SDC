#include "Bicycle.hpp"
#include <iostream>

Bicycle::Bicycle(double speed) : speed_(speed) {}

Bicycle::~Bicycle() = default;

void Bicycle::drive() {
    std::cout << "Bicycle is driving at speed: " << speed_ << " km/h\n";
}
