#include "Player/PlayerJudgeAbyssDeadStatus.h"

#include "Player/PlayerJudgeSameNerve.h"
#include "Player/PlayerRecoverySafetyPoint.h"
#include "Util/JudgeUtil.h"

PlayerJudgeAbyssDeadStatus::PlayerJudgeAbyssDeadStatus(const PlayerJudgeSameNerve* nerve,
                                                       const PlayerRecoverySafetyPoint* safePoint)
    : mNerve(nerve), mSafePoint(safePoint) {}

bool PlayerJudgeAbyssDeadStatus::judge() const {
    return rs::isJudge(mNerve) && !mSafePoint->isValid();
}

void PlayerJudgeAbyssDeadStatus::reset() {}

void PlayerJudgeAbyssDeadStatus::update() {}
