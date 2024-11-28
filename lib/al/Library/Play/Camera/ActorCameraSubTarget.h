#pragma once

#include <math/seadVector.h>
#include <Library/Camera/CameraSubTargetBase.h>

namespace al {
class LiveActor;

class ActorCameraSubTarget : public CameraSubTargetBase {
public:
    ActorCameraSubTarget(const LiveActor*);
    const char* getTargetName() const override;

    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;

private:
    const LiveActor* mActor;
    const sead::Vector3f* _20;
};

static_assert(sizeof(ActorCameraSubTarget) == 0x28);
}
