#include <memory>
#include "ShoppingCart.hpp"
#include "InvoicePrinter.hpp"
#include "CartRepository.hpp"
#include "Product.hpp"

int main() {
    ShoppingCart cart;
    InvoicePrinter printer;
    CartRepository repo;

    cart.addProduct(std::make_unique<Product>("Laptop", 50000));
    cart.addProduct(std::make_unique<Product>("Mouse", 2000));

    printer.print(cart);
    repo.save(cart);

    return 0;
}
