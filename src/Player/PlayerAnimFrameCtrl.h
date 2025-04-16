#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}

class PlayerAnimFrameCtrl {
public:
    PlayerAnimFrameCtrl();

    void startAction(al::LiveActor* player, const sead::SafeString& name);
    void changeActionName(al::LiveActor* player, const sead::SafeString& name);
    void update();
    void updateSync(al::LiveActor* player);
    void setFrame(f32 frame);

    const char* getActionName() const;
    f32 getCurrentFrame() const;
    f32 getRate() const;
    bool isActionEnd() const;

private:
    sead::FixedSafeString<64> mActionName = {""};
    bool mIsActionRepeat = false;
    f32 mCurrentFrame = 0.0f;
    f32 mActionFrameMax = 0.0f;
    f32 mRate = 1.0f;
    bool mIsSkipUpdate = false;
};

static_assert(sizeof(PlayerAnimFrameCtrl) == 0x70);
