#pragma once

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace al {
class KeyMoveFollowTarget : public LiveActor {
public:
    KeyMoveFollowTarget(const char* name);

    void initKeyMoveFollowTarget(const al::ActorInitInfo& info, const char* archiveName,
                                 const char* suffix);
    void exeWait();
    void exeMove();
    void exeStop();
    void init(const al::ActorInitInfo& info) override;

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mMoveWaitTime = 30;
    s32 mTimer = 0;
};

static_assert(sizeof(KeyMoveFollowTarget) == 0x118);

namespace {
NERVE_ACTION_IMPL(KeyMoveFollowTarget, Wait)
NERVE_ACTION_IMPL(KeyMoveFollowTarget, Move)
NERVE_ACTION_IMPL(KeyMoveFollowTarget, Stop)

NERVE_ACTIONS_MAKE_STRUCT(KeyMoveFollowTarget, Wait, Move, Stop)
}  // namespace
}  // namespace al
