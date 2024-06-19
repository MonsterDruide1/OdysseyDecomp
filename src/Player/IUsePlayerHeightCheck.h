#pragma once

#include <basis/seadTypes.h>

class IUsePlayerHeightCheck {
public:
    virtual bool isAboveGround() const = 0;
    virtual f32 getGroundHeight() const = 0;
    virtual f32 getShadowDropHeight() const = 0;
};
