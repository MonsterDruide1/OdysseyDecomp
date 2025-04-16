#pragma once

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
struct ActorParamMove;
}  // namespace al

class FlyerStateWanderParam {
public:
    FlyerStateWanderParam(s32, s32 wanderTime, s32 waitTime, const char* actionName,
                          const al::ActorParamMove* actorParamMove);

    s32 get_0() const { return _0; }

    s32 getWanderTime() const { return mWanderTime; }

    s32 getWaitTime() const { return mWaitTime; }

    const char* getActionName() const { return mActionName.cstr(); }

    const al::ActorParamMove* getActorParamMove() const { return mActorParamMove; }

private:
    s32 _0;
    s32 mWanderTime;
    s32 mWaitTime;
    sead::FixedSafeString<32> mActionName;
    const al::ActorParamMove* mActorParamMove;
};

static_assert(sizeof(FlyerStateWanderParam) == 0x50);

class FlyerStateWander : public al::ActorStateBase {
public:
    FlyerStateWander(al::LiveActor* actor, const FlyerStateWanderParam* param);

    void appear() override;

    void exeWander();
    void exeWait();

private:
    s32 mNerveTime = 0;
    sead::Vector3f mStartTrans = {0.0f, 0.0f, 0.0f};
    const FlyerStateWanderParam* mFlyerStateWanderParam;
};

static_assert(sizeof(FlyerStateWander) == 0x38);
