#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class PlayerInput;
class PlayerSeparateCapFlag;

class HackCapJudgePreInputSeparateJump : public al::HioNode, public IJudge {
public:
    HackCapJudgePreInputSeparateJump(const PlayerInput* input,
                                     const PlayerSeparateCapFlag* separateCapFlag);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const PlayerInput* mInput;
    const PlayerSeparateCapFlag* mSeparateCapFlag;
    int mCounter = 0;
};
