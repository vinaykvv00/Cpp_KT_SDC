#ifndef BICYCLE_HPP
#define BICYCLE_HPP

#include "IDriveable.hpp"

class Bicycle : public IDriveable {
private:
    double speed_;

public:
    explicit Bicycle(double speed);
    ~Bicycle() override;

    void drive() override;
};

#endif
