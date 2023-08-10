#pragma once

namespace al {
class RailRider;

class RailKeeper {
public:
    virtual al::RailRider* getRailRider() const;

private:
    al::RailRider* mRailRider;
};
}  // namespace al
