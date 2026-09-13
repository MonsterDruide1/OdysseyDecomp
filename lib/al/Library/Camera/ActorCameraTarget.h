#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Camera/CameraTargetBase.h"

namespace al {
class LiveActor;

class ActorCameraTarget : public CameraTargetBase {
public:
    ActorCameraTarget(const LiveActor* actor, f32 yOffset, const sead::Vector3f* localOffset);

    ActorCameraTarget(const LiveActor* actor, const sead::Vector3f* localOffset = nullptr)
        : mActor(actor), mLocalOffset(localOffset), mYOffset(0.0f) {}

    const char* getTargetName() const override;
    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcGravity(sead::Vector3f* gravity) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;
    bool isCollideGround() const override;
    bool isInWater() const override;

    const LiveActor* getActor() const { return mActor; }

private:
    const LiveActor* mActor;
    const sead::Vector3f* mLocalOffset;
    f32 mYOffset;
};

static_assert(sizeof(ActorCameraTarget) == 0x28);

class ActorMatrixCameraTarget : public ActorCameraTarget {
public:
    ActorMatrixCameraTarget(const LiveActor* actor, const sead::Matrix34f* matrix);

    void calcTrans(sead::Vector3f* trans) const override { mMatrix->getBase(*trans, 3); }

    void calcSide(sead::Vector3f* side) const override {
        mMatrix->getBase(*side, 0);
        f32 len = side->length();
        if (len > 0.0f)
            *side *= 1.0f / len;
    }

    void calcUp(sead::Vector3f* up) const override {
        mMatrix->getBase(*up, 1);
        f32 len = up->length();
        if (len > 0.0f)
            *up *= 1.0f / len;
    }

    void calcFront(sead::Vector3f* front) const override {
        mMatrix->getBase(*front, 2);
        f32 len = front->length();
        if (len > 0.0f)
            *front *= 1.0f / len;
    }

    void calcVelocity(sead::Vector3f* velocity) const override { velocity->set(0.0f, 0.0f, 0.0f); }

private:
    const sead::Matrix34f* mMatrix;
};

static_assert(sizeof(ActorMatrixCameraTarget) == 0x30);

}  // namespace al
