#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Product {
public:
    string name;
    double price;

    Product(string name, double price) : name(name), price(price) {}
};

class ShoppingCart {
private:
    vector<Product*> products;

public:
    void addProduct(Product* p) {
        products.push_back(p);
    }

    vector<Product*>& getProducts() {
        return products;
    }

    double calculateTotal() {
        double total = 0;
        for (auto p : products) {
            total += p->price;
        }
        return total;
    }

    void printInvoice() {
        cout << "Shopping Cart Invoice:\n";
        for (auto p : products) {
            cout << p->name << " - Rs " << p->price << endl;
        }
        cout << "Total: Rs " << calculateTotal() << endl;
    }

    void saveToDatabase() {
        cout << "Saving cart to database..." << endl;
    }
};

int main() {

    ShoppingCart cart;

    cart.addProduct(new Product("Laptop", 50000));
    cart.addProduct(new Product("Mouse", 2000));

    cart.printInvoice();    
    cart.saveToDatabase();  

    return 0;
}
