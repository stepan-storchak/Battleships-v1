#pragma once
#include <string>

class ICoordinate {
public:
    virtual ~ICoordinate() = default;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual std::string toString() const = 0;
    virtual bool operator==(const ICoordinate& other) const = 0;
};