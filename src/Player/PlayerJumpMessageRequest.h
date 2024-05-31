#pragma once

#include <math/seadVector.h>

enum class PlayerJumpType : u32 {
    Standard = 0,
    // more unknown
};

struct PlayerJumpMessageRequest {
    PlayerJumpMessageRequest();
    void clear();

    PlayerJumpType mJumpType = PlayerJumpType::Standard;
    f32 mJumpPower = 0;
    s32 mExtendFrame = 0;
    sead::Vector3f mTurnJumpAngle = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mActorTrans = {0.0f, 0.0f, 0.0f};
    bool mIsSpinClockwise = 0;
    bool mIsEnableStandUp = 0;
};
