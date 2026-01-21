#include "Player/PlayerJudgeAbyssDeadStatus.h"
#include "Player/PlayerInfo.h"
#include "Player/PlayerRecoverySafetyPoint.h"

#include "Util/JudgeUtil.h"

PlayerJudgeAbyssDeadStatus::PlayerJudgeAbyssDeadStatus(const PlayerJudgeSameNerve* nerve,
                                                       const PlayerRecoverySafetyPoint* safePoint)
    : mNerve(nerve), mSafePoint(safePoint) {}

bool PlayerJudgeAbyssDeadStatus::judge() const {
    if (!rs::isJudge(reinterpret_cast<const IJudge*>(mNerve)))
        return false;

    return !mSafePoint->isValid();
}

void PlayerJudgeAbyssDeadStatus::reset() {
    return;
}

void PlayerJudgeAbyssDeadStatus::update() {
    return;
}
