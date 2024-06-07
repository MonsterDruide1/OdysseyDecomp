#include "Player/PlayerJudgePreInputJump.h"

#include <math/seadMathCalcCommon.h>

#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"

PlayerJudgePreInputJump::PlayerJudgePreInputJump(const PlayerConst* pConst,
                                                 const PlayerInput* input,
                                                 IJudge* judgeForceSlopeSlide)
    : mConst(pConst), mInput(input), mJudgeForceSlopeSlide(judgeForceSlopeSlide) {}

void PlayerJudgePreInputJump::reset() {
    mRemainJumpFrame = 0;
}

void PlayerJudgePreInputJump::update() {
    mRemainJumpFrame = sead::Mathi::clampMin(mRemainJumpFrame - 1, 0);

    if (mInput->isTriggerJump()) {
        mRemainJumpFrame = mConst->getContinuousJumpPreInputFrame();
    }
}

bool PlayerJudgePreInputJump::judge() const {
    return mRemainJumpFrame > 0;
}
