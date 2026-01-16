#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;

class PlayerHitPush {
public:
    PlayerHitPush(const al::LiveActor* player, const PlayerConst* playerConst);
    void clearHitFlag();
    void setHitPush(const sead::Vector3f& dir);
    void setHitInfo(const sead::Vector3f& dir, f32 pushPower, f32 hopPower);
    void setHitPushStrong(const sead::Vector3f& dir);
    void setHitPushVeryStrong(const sead::Vector3f& dir);
    void setHitPushBlowDown(const sead::Vector3f& dir);

private:
    const al::LiveActor* mPlayer = nullptr;
    const PlayerConst* mPlayerConst = nullptr;
    bool mIsHit = false;
    bool mIsBlowDown = false;
    sead::Vector3f mPush = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(PlayerHitPush) == 0x20);
