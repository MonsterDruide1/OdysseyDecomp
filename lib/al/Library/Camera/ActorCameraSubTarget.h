#pragma once

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Camera/CameraSubTargetBase.h"

namespace al {
class LiveActor;

class ActorCameraSubTarget : public CameraSubTargetBase {
public:
    ActorCameraSubTarget(const LiveActor* actor);

    const char* getTargetName() const override;

    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;

    const LiveActor* getActor() const { return mActor; }

    void setOffset(const sead::Vector3f* offset) { mOffset = offset; }

private:
    const LiveActor* mActor;
    const sead::Vector3f* mOffset = nullptr;
};

static_assert(sizeof(ActorCameraSubTarget) == 0x28);

class ActorBackAroundCameraSubTarget : public ActorCameraSubTarget {
public:
    ActorBackAroundCameraSubTarget(const LiveActor* actor);

    void calcTrans(sead::Vector3f* trans) const override;

    const char* getTargetName() const override { return mTargetName.cstr(); }

private:
    sead::FixedSafeString<128> mTargetName{""};
};

static_assert(sizeof(ActorBackAroundCameraSubTarget) == 0xC0);

}  // namespace al
