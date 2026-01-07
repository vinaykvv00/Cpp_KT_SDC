#ifndef IFLYABLE_HPP
#define IFLYABLE_HPP

class IFlyable {
public:
    virtual void fly() = 0;
    virtual ~IFlyable() = default;
};

#endif
