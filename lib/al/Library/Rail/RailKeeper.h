#pragma once

#include "al/Library/Rail/RailRider.h"

namespace al {
class RailKeeper {
public:
    virtual al::RailRider* getRailRider() const;

    al::RailRider* mRailRider;  // _8
};
};  // namespace al
