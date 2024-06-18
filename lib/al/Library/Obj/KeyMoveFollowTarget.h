#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;

class KeyMoveFollowTarget : public LiveActor {
public:
    KeyMoveFollowTarget(const char* name);

    void initKeyMoveFollowTarget(const ActorInitInfo& info, const char* archiveName,
                                 const char* suffix);
    void init(const ActorInitInfo& info) override;
    void exeWait();
    void exeMove();
    void exeStop();

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mMoveWaitTime = 30;
    s32 mTimer = 0;
};

static_assert(sizeof(KeyMoveFollowTarget) == 0x118);
}  // namespace al
