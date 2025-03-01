#pragma once

#include <math/seadVector.h>

enum class PlayerJumpType : u32 {
    Standard = 0,
    // more unknown
};

struct PlayerJumpMessageRequest {
    PlayerJumpMessageRequest();
    void clear();

    PlayerJumpType jumpType = PlayerJumpType::Standard;
    f32 jumpPower = 0;
    s32 extendFrame = 0;
    sead::Vector3f turnJumpAngle = {0.0f, 0.0f, 0.0f};
    sead::Vector3f actorTrans = {0.0f, 0.0f, 0.0f};
    bool isSpinClockwise = 0;
    bool isEnableStandUp = 0;
};
