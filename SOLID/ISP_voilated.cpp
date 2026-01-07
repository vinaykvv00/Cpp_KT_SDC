#include <iostream>
#include <stdexcept>


class Vehicle {
public:
    virtual void drive() = 0;
    virtual void fly() = 0;   
    virtual void sail() = 0;  

    virtual ~Vehicle() = default;
};

class Bicycle : public Vehicle {
public:
    void drive() override {
        std::cout << "Bicycle is moving on road.\n";
    }

    void fly() override {
        throw std::logic_error("Bicycle cannot fly");
    }

    void sail() override {
        throw std::logic_error("Bicycle cannot sail");
    }
};

class Boat : public Vehicle {
public:
    void drive() override {
        throw std::logic_error("Boat cannot drive on road");
    }

    void fly() override {
        throw std::logic_error("Boat cannot fly");
    }

    void sail() override {
        std::cout << "Boat is sailing on water.\n";
    }
};

class Airplane : public Vehicle {
public:
    void drive() override {
        std::cout << "Airplane is taxiing on runway.\n";
    }

    void fly() override {
        std::cout << "Airplane is flying in the sky.\n";
    }

    void sail() override {
        throw std::logic_error("Airplane cannot sail");
    }
};

int main() {
    Vehicle* cycle = new Bicycle();
    Vehicle* boat = new Boat();
    Vehicle* plane = new Airplane();

    cycle->drive();
    boat->sail();
    plane->fly();

    try {
        cycle->fly();
    } catch (std::logic_error& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    delete cycle;
    delete boat;
    delete plane;
}
