#include "Player/PlayerJudgeAbyssDeadStatus.h"

#include "Util/JudgeUtil.h"

PlayerJudgeAbyssDeadStatus::PlayerJudgeAbyssDeadStatus(const PlayerJudgeSameNerve* nerve,
                                                       const PlayerRecoverySafetyPoint* safePoint)
    : mNerve(nerve), mSafePoint(safePoint) {}

bool PlayerJudgeAbyssDeadStatus::judge() const {
    return rs::isJudge(mNerve) && !mSafePoint->isValid();
}

void PlayerJudgeAbyssDeadStatus::reset() {
    return;
}

void PlayerJudgeAbyssDeadStatus::update() {
    return;
}
