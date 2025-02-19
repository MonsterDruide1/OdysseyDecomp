#pragma once

#include "Library/Camera/CameraTargetBase.h"

namespace al {
class CameraSubTargetTurnParam;

class CameraSubTargetBase : public CameraTargetBase {
public:
    CameraSubTargetBase();

    virtual void calcSide(sead::Vector3f* side) const override;
    virtual void calcUp(sead::Vector3f* up) const override;
    virtual void calcFront(sead::Vector3f* front) const override;
    virtual void calcVelocity(sead::Vector3f* velocity) const override;

private:
    CameraSubTargetTurnParam* mTurnParam;
};

static_assert(sizeof(CameraSubTargetBase) == 0x18);
}  // namespace al
