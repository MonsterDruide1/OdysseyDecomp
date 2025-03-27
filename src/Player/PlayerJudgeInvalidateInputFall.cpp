#include "Player/PlayerJudgeInvalidateInputFall.h"

#include <basis/seadTypes.h>

#include "Library/LiveActor/ActorPoseUtil.h"

#include "Player/PlayerAreaChecker.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeInvalidateInputFall::PlayerJudgeInvalidateInputFall(const al::LiveActor* actor,
                                                               const PlayerAreaChecker* areaChecker,
                                                               const IUsePlayerCollision* collision)
    : mActor(actor), mAreaChecker(areaChecker), mCollision(collision) {}

void PlayerJudgeInvalidateInputFall::reset() {
    mInvalidateInputFallArea = nullptr;
}

void PlayerJudgeInvalidateInputFall::update() {
    mInvalidateInputFallArea = nullptr;
    if (!rs::isCollidedGround(mCollision))
        mInvalidateInputFallArea = mAreaChecker->tryFindInvalidateInputFall(al::getTrans(mActor));
}

bool PlayerJudgeInvalidateInputFall::judge() const {
    return mInvalidateInputFallArea != nullptr;
}
