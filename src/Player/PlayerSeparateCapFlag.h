#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class PlayerSeparateCapFlag {
public:
    PlayerSeparateCapFlag();

    bool isEnableHipDrop() const;
    bool isEnableHipDropCancel() const;
    bool isEnableCapTouchJump() const;
    bool isEnableGroundPound() const;
    bool isSeparateMode() const;

    void setSeparateMode(bool);
    void setHipDrop(bool);
    void setCapTouchJump(bool);

private:
    bool mIsSeparateMode = false;
    bool mIsHipDrop = false;
    bool mIsHipDropCancel = false;
    char padding;
    u32 mIsCapTouchJump = 0;
    const al::LiveActor* mPlayer = nullptr;
};

static_assert(sizeof(PlayerSeparateCapFlag) == 0x10);
