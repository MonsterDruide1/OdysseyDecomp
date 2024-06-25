#pragma once

#include <math/seadVector.h>

#include "Player/IJudge.h"

class PlayerConst;
class PlayerInput;
class PlayerCarryKeeper;
class HackCap;

class PlayerJudgePreInputCapThrow : public IJudge {
public:
    PlayerJudgePreInputCapThrow(const PlayerConst* pConst, const PlayerInput* input,
                                const PlayerCarryKeeper* carryKeeper, const HackCap* hackCap);
    void recordJudgeAndReset();
    void recordSeparateJudge();
    void recordCooperateAndReset();
    void reset() override;
    void update() override;
    bool judge() const override;

public:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const PlayerCarryKeeper* mCarryKeeper;
    const HackCap* mHackCap;
    int mCounter1 = 0;
    int mCounter2 = 0;
    int mThrowType = 0;
    sead::Vector2f mCapThrowDir = {0.0f, 0.0f};
    sead::Vector2f _3C = {0.0f, 0.0f};
    bool _44 = 0;
    int _30_2 = 0;
    sead::Vector2f _34_2 = {0.0f, 0.0f};;
    sead::Vector2f _3C_2 = {0.0f, 0.0f};;
    bool _44_2 = 0;
};
