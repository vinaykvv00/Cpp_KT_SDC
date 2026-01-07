#pragma once
#include <vector>
#include <memory>
#include "Product.hpp"

class ShoppingCart {
public:
    ShoppingCart() = default;
    ~ShoppingCart() = default;

    void addProduct(std::unique_ptr<Product> product);
    [[nodiscard]] double calculateTotal() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Product>>& getProducts() const;

private:
    std::vector<std::unique_ptr<Product>> products_;
};
