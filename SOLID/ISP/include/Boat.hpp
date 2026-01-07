#ifndef BOAT_HPP
#define BOAT_HPP

#include "ISailable.hpp"

class Boat : public ISailable {
private:
    double displacement_;

public:
    explicit Boat(double displacement);
    ~Boat() override;

    void sail() override;
};

#endif
