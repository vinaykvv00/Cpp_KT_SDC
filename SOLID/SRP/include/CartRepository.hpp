#pragma once
#include "ShoppingCart.hpp"

class CartRepository {
public:
    CartRepository() = default;
    ~CartRepository() = default;

    void save(const ShoppingCart& cart) const;
};
