#pragma once

#include "Library/Area/AreaObj.h"

class ExtForceArea : public al::AreaObj {
public:
    ExtForceArea(const char* name);

    void init(const al::AreaInitInfo& info) override;

    void calcExtForce(sead::Vector3f* force, const sead::Vector3f& pos,
                      const sead::Vector3f& prevForce, const sead::Vector3f& velocity) const;

private:
    f32 mSpeedVStep = 0.0f;
    f32 mSpeedVTarget = 0.0f;
    f32 mForceH = 1.0f;
};
