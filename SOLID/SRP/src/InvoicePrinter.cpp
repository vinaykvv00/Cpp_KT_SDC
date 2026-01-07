#include "InvoicePrinter.hpp"
#include <iostream>

void InvoicePrinter::print(const ShoppingCart& cart) const {
    std::cout << "Shopping Cart Invoice:\n";

    for (const auto& p : cart.getProducts()) {
        std::cout << p->getName() << " - Rs " << p->getPrice() << "\n";
    }

    std::cout << "Total: Rs " << cart.calculateTotal() << "\n";
}
