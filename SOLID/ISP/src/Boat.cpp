#include "Boat.hpp"
#include <iostream>

Boat::Boat(double displacement) : displacement_(displacement) {}

Boat::~Boat() = default;

void Boat::sail() {
    std::cout << "Boat is sailing with displacement: " << displacement_ << " tons\n";
}
