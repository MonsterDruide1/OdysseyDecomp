#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class PlayerSeparateCapFlag {
public:
    PlayerSeparateCapFlag(al::LiveActor* player);

    bool isEnableHipDrop() const;
    bool isEnableHipDropCancel() const;
    bool isEnableCapTouchJump() const;
    bool isEnableGroundPound() const;
    bool isSeparateMode() const;

    void setSeparateMode(bool);
    void setHipDrop(bool);
    void setCapTouchJump(bool);

private:
    const al::LiveActor* mPlayer;
    bool mIsSeparateMode;
    bool mIsHipDrop;
    bool mIsHipDropCancel;
    bool mIsCapTouchJump;
};

static_assert(sizeof(PlayerSeparateCapFlag) == 0x10);
