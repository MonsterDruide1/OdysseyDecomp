#include "Player/PlayerJudgeLongFall.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/IUsePlayerFallDistanceCheck.h"
#include "Player/PlayerBindKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerHackKeeper.h"

PlayerJudgeLongFall::PlayerJudgeLongFall(const al::LiveActor* player, const PlayerConst* pConst,
                                         const IUsePlayerFallDistanceCheck* fallDistanceCheck,
                                         const PlayerHackKeeper* hackKeeper,
                                         const PlayerBindKeeper* bindKeeper,
                                         const IPlayerModelChanger* modelChanger)
    : mPlayer(player), mConst(pConst), mFallDistanceCheck(fallDistanceCheck),
      mHackKeeper(hackKeeper), mBindKeeper(bindKeeper), mModelChanger(modelChanger) {}

bool PlayerJudgeLongFall::judge() const {
    if (PlayerFunction::isPlayerDeadStatus(mPlayer))
        return false;

    if (mModelChanger->is2DModel())
        return false;
    if (mBindKeeper->getBindSensor() )
        return false;
    if (mHackKeeper->getUnkHitSensor() )
        return false;

    f32 fallen = mFallDistanceCheck->getFallDistance();
    f32 threshold = mConst->getLongFallDistance();
    return fallen >= threshold;
}
