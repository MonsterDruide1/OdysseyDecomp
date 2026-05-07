#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class PlayerFormSensorCollisionArranger {
public:
    const sead::Vector3f& getHeadPos() const;
    const sead::Vector3f& getBodyPos() const;

    s32 getFormStatus() const { return mFormStatus; }

private:
    u8 _0[0x24];
    s32 mFormStatus = 0;
};
