#pragma once

#include <basis/seadTypes.h>

class IUsePlayerCeilingCheck {
public:
    virtual bool isEnableStandUp() const = 0;
    virtual bool isEnableHoldUp() const = 0;
    virtual bool isPressedCeil() const = 0;
    virtual f32 getSafetyCeilSpace() const = 0;
    virtual f32 getCeilCheckHeight() const = 0;
};
