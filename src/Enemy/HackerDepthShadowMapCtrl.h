#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerCollider;

class HackerDepthShadowMapCtrl {
public:
    HackerDepthShadowMapCtrl(al::LiveActor* actor, const char* shadowMapName,
                             f32 shadowLengthOffset, f32 lerpFactorX, f32 lerpFactorY);
    void resetAndUpdate();
    void update(PlayerCollider* playerCollider);

private:
    al::LiveActor* mActor = nullptr;
    const char* mShadowMapName = nullptr;
    f32 mSmoothedLength = 0.0f;
    bool mIsActive = true;
    f32 mShadowLengthOffset = 0.0f;
    s32 mOnGroundTimer = 0;
    sead::Vector2f mLerpedLength = {0.0f, 0.0f};
};
