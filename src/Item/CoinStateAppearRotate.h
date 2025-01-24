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
                          const sead::Vector3f& trans, const char* name);

    void appear() override;
    void exeRotate();

private:
    al::MtxConnector* mMtxConnector;
    sead::Vector3f mVelocity = sead::Vector3f::zero;
    sead::Vector3f mPosition = sead::Vector3f::zero;
    f32 mTransY = 0.0f;
    const sead::Vector3f& mTrans;
    const char* mName;
};
