#pragma once

#include <math/seadVector.h>

enum class PlayerJumpType : u32 {
    Standard = 0,
    val_01 = 1,
    val_02 = 2,
    val_03 = 3,
    val_04 = 4,
    val_05 = 5,
    val_06 = 6,
    val_07 = 7,
    val_08 = 8,
    val_09 = 9,
    val_0A = 10,
    val_0B = 11,
    val_0C = 12,
    val_0D = 13,
    val_0E = 14,
    val_0F = 15,
    TurnJump = 16,
    HipDropJump = 17,
    val_12 = 18,
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
