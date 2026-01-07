#ifndef AIRPLANE_HPP
#define AIRPLANE_HPP

#include "IDriveable.hpp"
#include "IFlyable.hpp"

class Airplane : public IDriveable, public IFlyable {
private:
    double wingspan_;

public:
    explicit Airplane(double wingspan);
    ~Airplane() override;

    void drive() override; 
    void fly() override;
};

#endif
