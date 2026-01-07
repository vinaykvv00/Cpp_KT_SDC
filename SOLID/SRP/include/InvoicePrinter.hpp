#pragma once
#include "ShoppingCart.hpp"

class InvoicePrinter {
public:
    InvoicePrinter() = default;
    ~InvoicePrinter() = default;

    void print(const ShoppingCart& cart) const;
};
