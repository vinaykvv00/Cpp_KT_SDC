#include "ShoppingCart.hpp"

void ShoppingCart::addProduct(std::unique_ptr<Product> product) {
    products_.push_back(std::move(product));
}

double ShoppingCart::calculateTotal() const {
    double total = 0.0;
    for (const auto& p : products_) {
        total += p->getPrice();
    }
    return total;
}

const std::vector<std::unique_ptr<Product>>& ShoppingCart::getProducts() const {
    return products_;
}
