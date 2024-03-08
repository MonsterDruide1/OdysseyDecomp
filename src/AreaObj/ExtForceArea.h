#pragma once

#include "Library/Area/AreaObj.h"

class ExtForceArea : public al::AreaObj {
public:
    ExtForceArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

    void calcExtForce(sead::Vector3f, sead::Vector3f, sead::Vector3f, sead::Vector3f);

private:
    sead::Vector2f mUnknown;
    f32 mMagnitude = 1.0;
};
