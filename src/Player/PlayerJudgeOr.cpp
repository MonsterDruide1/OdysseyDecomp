#include "Player/PlayerJudgeOr.h"

#include "Util/JudgeUtil.h"

PlayerJudgeOr::PlayerJudgeOr(s32 maxJudges) {
    mJudges.allocBuffer(maxJudges, nullptr);
}

void PlayerJudgeOr::addJudge(IJudge* judge) {
    mJudges.pushBack(judge);
}

bool PlayerJudgeOr::judge() const {
    for (auto& judge : mJudges)
        if (rs::updateJudgeAndResult(&judge))
            return true;

    return false;
}
