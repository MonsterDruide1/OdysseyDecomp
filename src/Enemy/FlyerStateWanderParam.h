#pragma once

#include <prim/seadSafeString.h>

namespace al {
struct ActorParamMove;
}

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
