#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class ScreenPointAnalyzer {
public:
    ScreenPointAnalyzer();
    void reset();
    void update();
    void analyze(const sead::Vector2f& pos);
    bool isHold() const;
    bool isSlide() const;

    s32 _00 = 0;
    s32 _04 = 0;
    s32 _08 = 0;
    s32 _0c = 0;
    s32 _10 = 0;
    s32 _14 = 0;
    sead::Vector2f mSlideDir = sead::Vector2f::zero;
    sead::Vector2f mMoveVec = sead::Vector2f::zero;
    s32 _28 = 0;
    s32 _2c = 0;
    f32 mRotateSpeedDegree = 0.0f;
};

static_assert(sizeof(ScreenPointAnalyzer) == 0x34);
