#pragma once

namespace al {
class RailRider;

class IUseRail {
public:
    virtual RailRider* getRailRider() const = 0;
};
}  // namespace al
