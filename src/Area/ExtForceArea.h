#pragma once

#include "Library/Area/AreaObj.h"

class ExtForceArea : public al::AreaObj {
public:
    ExtForceArea(const char* name);

    void init(const al::AreaInitInfo& info) override;

    void calcExtForce(sead::Vector3f* pOutForce, const sead::Vector3f& pos,
                      const sead::Vector3f& velocity, const sead::Vector3f& prevVelocity) const;

private:
    f32 mSpeedRangeX = 0.0f;
    f32 mSpeedRangeY = 0.0f;
    f32 mMagnitude = 1.0f;
};
