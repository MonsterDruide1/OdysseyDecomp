#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
class MtxConnector;
}  // namespace al

class CoinStateAppearRotate : public al::ActorStateBase {
public:
    CoinStateAppearRotate(al::LiveActor* actor, al::MtxConnector* mtxConnector,
                          const sead::Vector3f& displayOffset, const char* rotateHitReaction);

    void appear() override;
    void exeRotate();

private:
    al::MtxConnector* mMtxConnector;
    sead::Vector3f mVelocity = sead::Vector3f::zero;
    sead::Vector3f mOffset = sead::Vector3f::zero;
    f32 mInitialTransY = 0.0f;
    const sead::Vector3f& mDisplayOffset;
    const char* mRotateHitReaction;
};
