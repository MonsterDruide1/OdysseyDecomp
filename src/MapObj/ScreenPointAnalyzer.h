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

    const sead::Vector2f& getSlideDir() const { return mSlideDir; }

    const sead::Vector2f& getMoveVec() const { return mMoveVec; }

    f32 getRotateSpeedDegree() const { return mRotateSpeedDegree; }

private:
    s32 _00 = 0;
    s32 _04 = 0;
    s32 _08 = 0;
    s32 _0c = 0;
    s32 _10 = 0;
    s32 _14 = 0;
    sead::Vector2f mSlideDir = {0.0f, 0.0f};
    sead::Vector2f mMoveVec = {0.0f, 0.0f};
    s32 _28 = 0;
    s32 _2c = 0;
    f32 mRotateSpeedDegree = 0.0f;
};

static_assert(sizeof(ScreenPointAnalyzer) == 0x34);
