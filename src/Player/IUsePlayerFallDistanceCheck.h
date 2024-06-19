#pragma once

#include <basis/seadTypes.h>

class IUsePlayerFallDistanceCheck {
public:
    virtual f32 getFallDistance() const = 0;
};
