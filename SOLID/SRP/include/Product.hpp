#pragma once
#include <string>

class Product {
    private:
        std::string name_;
        double price_{0.0};
    public:
        Product() = default;
        Product(const std::string name, double price);
        ~Product() = default;
         
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] double getPrice() const;
};