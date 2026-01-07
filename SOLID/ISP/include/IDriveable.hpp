#ifndef IDRIVEABLE_HPP
#define IDRIVEABLE_HPP

class IDriveable {
public:
    virtual void drive() = 0;
    virtual ~IDriveable() = default;
};

#endif
