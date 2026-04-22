#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class ExternalForceKeeper;
class IUsePlayerCollision;

class PlayerExternalVelocity {
public:
    bool isExistForce() const;
    bool isExistSnapForce() const;

private:
    ExternalForceKeeper* mExternalForceKeeper;
    sead::Vector3f _8;
    sead::Vector3f _14;
    sead::Vector3f _20;
    const al::LiveActor* mActor;
    const IUsePlayerCollision* mCollision;
    const sead::Vector3f* _38;
    bool _40;
    sead::Vector3f _44;
    f32 _50[6];
    sead::Vector3f mSnapForce;
    s32 mApplyLastGroundInertia;
    sead::Vector3f _80;
    sead::Vector3f _8c;
};

static_assert(sizeof(PlayerExternalVelocity) == 0x98);
