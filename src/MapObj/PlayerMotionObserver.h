#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class PlayerHolder;
}  // namespace al

class SaveObjInfo;

class PlayerMotionObserver : public al::LiveActor {
public:
    PlayerMotionObserver(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeCheckLand();
    void exeWaitForSwitchOn();

private:
    al::PlayerHolder* mPlayerHolder = nullptr;
    s32 mSwitchOnDelayFrameNum = 0;
    s32 mSwitchOnDelayFrame = 0;
    SaveObjInfo* mSaveObjInfo = nullptr;
    bool mIsUseSwitchStart = false;
};

static_assert(sizeof(PlayerMotionObserver) == 0x128);
