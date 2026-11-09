#pragma once

namespace al {
class LiveActor;
}

class FrogParam {
public:
    FrogParam(al::LiveActor* actor);

    f32* mGravity = nullptr;                     // 0x00, "重力"
    f32* mFrictionNormal = nullptr;              // 0x08, "摩擦[通常]"
    f32* mFrictionWalk = nullptr;                // 0x10, "摩擦[歩き]"
    f32* mFrictionAir = nullptr;                 // 0x18, "摩擦[空中]"
    f32* mWalkAccel = nullptr;                   // 0x20, "歩き加速度"
    f32* mWalkMinSpeed = nullptr;                // 0x28, "歩き最小速度"
    f32* mWalkMaxSpeed = nullptr;                // 0x30, "歩き最大速度"
    f32* mRunAccel = nullptr;                    // 0x38, "走り加速度(=Yダッシュ)"
    f32* mRunMaxSpeed = nullptr;                 // 0x40, "走り最大速度"
    f32* mWalkStartAccel = nullptr;              // 0x48, "歩きだし加速度"
    f32* mWalkStartMaxSpeed = nullptr;           // 0x50, "歩きだし最大速度"
    f32* mJumpMinHeight = nullptr;               // 0x58, "ジャンプ[最小高さ]"
    f32* mJumpMaxHeight = nullptr;               // 0x60, "ジャンプ[最大高さ]"
    f32* mSwingJumpMinHeight = nullptr;          // 0x68, "振り入力ジャンプ[最小高さ]"
    f32* mSwingJumpMaxHeight = nullptr;          // 0x70, "振り入力ジャンプ[最大高さ]"
    f32* mTrampleJumpMinHeight = nullptr;        // 0x78, "踏みジャンプ[最小高さ]"
    f32* mTrampleJumpMaxHeight = nullptr;        // 0x80, "踏みジャンプ[最大高さ]"
    f32* mTapJumpMinHeight = nullptr;            // 0x88, "チョイ押しジャンプ[最小高さ]"
    f32* mTapJumpMaxHeight = nullptr;            // 0x90, "チョイ押しジャンプ[最大高さ]"
    f32* mJumpPowerReturnRate = nullptr;         // 0x98, "ジャンプ[ジャンプ力返還率]"
    f32* mJumpAdjustTime = nullptr;              // 0xa0, "ジャンプ[調整時間]"
    f32* mJumpMinSpeed = nullptr;                // 0xa8, "ジャンプ[最小速度]"
    f32* mJumpMaxSpeed = nullptr;                // 0xb0, "ジャンプ[最大速度]"
    f32* mJumpFrontMove = nullptr;               // 0xb8, "ジャンプ[前方移動]"
    f32* mJumpBackMove = nullptr;                // 0xc0, "ジャンプ[後方移動]"
    f32* mJumpTurnMove = nullptr;                // 0xc8, "ジャンプ[旋回移動]"
    f32* mJumpGravity = nullptr;                 // 0xd0, "ジャンプ[重力]"
    f32* mHackEndJumpSpeed = nullptr;            // 0xd8, "キャプチャー解除ジャンプ速度"
    f32* mSwimAccel = nullptr;                   // 0xe0, "泳ぎ[加速度]"
    f32* mSwimMaxSpeed = nullptr;                // 0xe8, "泳ぎ[最高速度]"
    f32* mSwimFrictionWaterSurface = nullptr;    // 0xf0, "泳ぎ[摩擦率[水面]"
    f32* mSwimAccelDash = nullptr;               // 0xf8, "泳ぎ[加速度](Yダッシュ)"
    f32* mSwimMaxSpeedDash = nullptr;            // 0x100, "泳ぎ[最高速度](Yダッシュ)"
    f32* mSwimFrictionWaterSurfaceDash = nullptr;  // 0x108, "泳ぎ[摩擦率[水面](Yダッシュ)"
    f32* mSwimAnimRateDash = nullptr;            // 0x110, "泳ぎ[アニメレイト](Yダッシュ)"
    f32* mSwimReturnNormalSpeed = nullptr;       // 0x118, "泳ぎ[通常泳ぎに戻る速度]"
};

static_assert(sizeof(FrogParam) == 0x120);
