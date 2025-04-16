#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class PlayerConst;
class PlayerInput;

class PlayerJudgePreInputJump : public al::HioNode, public IJudge {
public:
    PlayerJudgePreInputJump(const PlayerConst* pConst, const PlayerInput* input,
                            IJudge* judgeForceSlopeSlide);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    IJudge* mJudgeForceSlopeSlide;
    s32 mRemainJumpFrame = 0;
};
