#include "rs/Judge.h"

#include "game/judge/IJudge.h"

void rs::resetJudge(IJudge* judge) {
    judge->reset();
}

void rs::updateJudge(IJudge* judge) {
    judge->update();
}

bool rs::isJudge(const IJudge* judge) {
    return judge->judge();
}

bool rs::updateJudgeAndResult(IJudge* judge) {
    judge->update();
    return judge->judge();
}

bool rs::judgeAndResetReturnTrue(IJudge* judge) {
    if (!judge->judge()) {
        return false;
    }

    judge->reset();

    return true;
}
