#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerCollider;

class HackerDepthShadowMapCtrl {
public:
    HackerDepthShadowMapCtrl(al::LiveActor* actor, const char* shadowMapName,
                             f32 shadowLengthOffset, f32 lerpLength, f32 lerpGradationLength);
    void resetAndUpdate();
    void update(PlayerCollider* playerCollider);

private:
    al::LiveActor* mActor = nullptr;
    const char* mShadowMapName = nullptr;
    f32 mGradationLength = 0.0f;
    bool mIsActive = true;
    f32 mShadowLengthOffset = 0.0f;
    s32 mOnGroundTimer = 0;
    f32 mLerpLength = 0.0f;
    f32 mLerpGradationLength = 0.0f;
};
