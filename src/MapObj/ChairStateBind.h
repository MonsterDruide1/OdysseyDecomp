#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerPuppet;

class ChairStateBindSitDown : public al::ActorStateBase {
public:
    ChairStateBindSitDown(al::LiveActor* actor, IUsePlayerPuppet** playerPuppet,
                          const char* startAction1, const char* startAction2,
                          const char* sitDownAction);
    void appear() override;
    void exeWaitPlayerLand();
    void exeSitDownStart();
    void exeSitDown();

    s32 getResult() const { return mResult; }

    void setUseLongStartStep(bool useLongStartStep) { mIsLongStartStep = useLongStartStep; }

    void setEnableStartInterpolation(bool enableStartInterpolation) {
        mIsStartInterpolationEnabled = enableStartInterpolation;
    }

private:
    IUsePlayerPuppet** mPlayerPuppet = nullptr;
    s32 mResult = 3;
    sead::Vector3f mStartOffset = sead::Vector3f::zero;
    bool mIsMoveX = false;
    sead::Quatf mStartPuppetQuat = sead::Quatf::unit;
    sead::Vector3f mStartPuppetTrans = sead::Vector3f::zero;
    const char* mStartAction1 = nullptr;
    const char* mStartAction2 = nullptr;
    const char* mSitDownAction = nullptr;
    bool _70 = false;
    bool _71 = false;
    bool mIsLongStartStep = false;
    bool _73 = false;
    s32 mStartStepMax = 0;
    bool mIsStartInterpolationEnabled = true;
    bool _79 = false;
    bool _7a = false;
    bool _7b = false;
    bool _7c = false;
    bool _7d = false;
    bool _7e = false;
    bool _7f = false;
};

static_assert(sizeof(ChairStateBindSitDown) == 0x80);

class ChairStateBindStandUp : public al::ActorStateBase {
public:
    ChairStateBindStandUp(al::LiveActor* actor, IUsePlayerPuppet** playerPuppet);
    void appear() override;
    void exeStandUp();

    s32 getResult() const { return mResult; }

private:
    IUsePlayerPuppet** mPlayerPuppet = nullptr;
    s32 mResult = 3;
};

static_assert(sizeof(ChairStateBindStandUp) == 0x30);

class ChairStateBindJump : public al::ActorStateBase {
public:
    ChairStateBindJump(al::LiveActor* actor, IUsePlayerPuppet** playerPuppet);
    void appear() override;
    void exeJump();

private:
    IUsePlayerPuppet** mPlayerPuppet = nullptr;
};

static_assert(sizeof(ChairStateBindJump) == 0x28);

namespace rs {
bool tryEndBindCapThrowOnChair(IUsePlayerPuppet** playerPuppet);
bool tryBindJumpOnChair(IUsePlayerPuppet* playerPuppet);
bool tryBindStandUpOnChair(IUsePlayerPuppet* playerPuppet);
}  // namespace rs
