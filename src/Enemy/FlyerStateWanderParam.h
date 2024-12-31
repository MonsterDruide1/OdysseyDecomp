#pragma once

#include <prim/seadSafeString.h>

namespace al {
// TODO: Move this in correct header.
struct ActorParamMove {
public:
    f32 ukn[4];
};
}  // namespace al

class FlyerStateWanderParam {
public:
    FlyerStateWanderParam(s32, s32, s32, const char* actionName,
                          const al::ActorParamMove* actorParamMove);

    s32 get_0() const { return _0; }

    s32 get_4() const { return _4; }

    s32 get_8() const { return _8; }

    const char* getActionName() const { return mActionName.cstr(); }

    al::ActorParamMove* getActorParamMove() const { return mActorParamMove; }

private:
    s32 _0;
    s32 _4;
    s32 _8;
    sead::FixedSafeString<32> mActionName;
    al::ActorParamMove* mActorParamMove;
};

static_assert(sizeof(FlyerStateWanderParam) == 0x50);
