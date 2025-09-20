#pragma once

#include "Library/Camera/CameraTargetBase.h"

namespace al {
class CameraSubTargetTurnParam;

class CameraSubTargetBase : public CameraTargetBase {
public:
    CameraSubTargetBase();

    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;

private:
    CameraSubTargetTurnParam* mTurnParam;
};

static_assert(sizeof(CameraSubTargetBase) == 0x18);
}  // namespace al
