#include "Player/PlayerJudgeStartRise.h"

#include "Library/LiveActor/ActorPoseUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerAreaChecker.h"

PlayerJudgeStartRise::PlayerJudgeStartRise(const al::LiveActor* player,
                                           const PlayerAreaChecker* areaChecker,
                                           const IPlayerModelChanger* modelChanger)
    : mPlayer(player), mAreaChecker(areaChecker), mModelChanger(modelChanger) {}

void PlayerJudgeStartRise::reset() {}

void PlayerJudgeStartRise::update() {}

bool PlayerJudgeStartRise::judge() const {
    if (mModelChanger->is2DModel())
        return false;
    return mAreaChecker->isInRise(al::getTrans(mPlayer));
}
