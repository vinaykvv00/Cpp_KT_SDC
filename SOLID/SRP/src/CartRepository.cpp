#include "CartRepository.hpp"
#include <iostream>

void CartRepository::save(const ShoppingCart&) const {
    std::cout << "Saving shopping cart to database...\n";
}
