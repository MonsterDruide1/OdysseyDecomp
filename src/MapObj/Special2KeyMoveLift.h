#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/MapObj/KeyMoveMapParts.h"

namespace al {
class KeyPoseKeeper;
}  // namespace al

class Special2KeyMoveParts : public al::LiveActor {
public:
    Special2KeyMoveParts(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void start();
    void reset();
    void exeStandBy();
    void exeDelay();
    void exeMoveSign();
    void exeMove();
    void exeStop();
    void exeReset();

private:
    al::KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mDelayTime = 0;
    s32 mMoveTime = 0;
};

static_assert(sizeof(Special2KeyMoveParts) == 0x118);

class Special2KeyMoveLift : public al::KeyMoveMapParts {
public:
    Special2KeyMoveLift(const char* name);

    void exeStandBy() override;

private:
    bool mIsFirstStandBy = true;
};

static_assert(sizeof(Special2KeyMoveLift) == 0x158);
