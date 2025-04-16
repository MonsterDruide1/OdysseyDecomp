#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SimpleAudioUser;

class GateMapParts : public LiveActor {
public:
    GateMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    void appearAndSetStart();
    void exeWait();
    void exeOpen();
    void updatePose(f32 rate);
    void exeBound();
    void exeEnd();

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Quatf mMoveNextQuat = sead::Quatf::unit;
    sead::Vector3f mMoveNextTrans = sead::Vector3f::zero;
    s32 mMaxHitReactions = 10;
    s32 mOpenTime = 120;
    f32 mBoundRate = 0.1f;
    s32 mCurrentBoundSteps = 120;
    s32 mHitReactionCurrent = 0;
    f32 mCurrentBoundRate = 0.1f;
    s32 mHitReactionCount = 0;
    SimpleAudioUser* mSuccessSeObj = nullptr;
};

static_assert(sizeof(GateMapParts) == 0x168);
}  // namespace al
