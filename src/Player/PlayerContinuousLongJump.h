#pragma once

class PlayerConst;

class PlayerContinuousLongJump {
public:
    PlayerContinuousLongJump(const PlayerConst* pConst);

    void countUp();
    void update();
    const char* getLongJumpAnimName() const;

private:
    const PlayerConst* mConst;
    s32 mCount = 0;
    s32 mTimer = 0;
};
