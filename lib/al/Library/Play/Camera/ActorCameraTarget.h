#pragma once

#include "Library/Camera/CameraTargetBase.h"

namespace al {
class LiveActor;

class ActorCameraTarget : public CameraTargetBase {
public:
    ActorCameraTarget(const LiveActor* actor, f32 yOffset, const sead::Vector3f* localOffset);

    const char* getTargetName() const override;
    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcGravity(sead::Vector3f* gravity) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;
    bool isCollideGround() const override;
    bool isInWater() const override;

private:
    const LiveActor* mActor;
    const sead::Vector3f* mLocalOffset;
    f32 mYOffset;
};

}  // namespace al
