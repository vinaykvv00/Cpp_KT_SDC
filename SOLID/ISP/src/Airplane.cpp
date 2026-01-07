#include "Airplane.hpp"
#include <iostream>

Airplane::Airplane(double wingspan) : wingspan_(wingspan) {}

Airplane::~Airplane() = default;

void Airplane::drive() {
    std::cout << "Airplane is taxiing on runway. Wingspan: " << wingspan_ << " meters\n";
}

void Airplane::fly() {
    std::cout << "Airplane is flying. Wingspan: " << wingspan_ << " meters\n";
}
