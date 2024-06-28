#include "Player/PlayerJudgeInvalidateInputFall.h"

#include <basis/seadTypes.h>
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "PlayerUtil.h"
#include "Stuff.h"

PlayerJudgeInvalidateInputFall::PlayerJudgeInvalidateInputFall(const al::LiveActor* actor,
                                                               const PlayerAreaChecker* areaChecker,
                                                               const IUsePlayerCollision* collision) : mActor(actor),
                                                                                                      mAreaChecker(areaChecker),
                                                                                                      mCollision(collision) {}
void PlayerJudgeInvalidateInputFall::reset() {
    mInvalidateInputFallArea = nullptr;
}
void PlayerJudgeInvalidateInputFall::update() {
    reset();
    if(rs::isCollidedGround(mCollision)) return;
    mInvalidateInputFallArea = mAreaChecker->tryFindInvalidateInputFall(al::getTrans(mActor));
}
bool PlayerJudgeInvalidateInputFall::judge() const {
    return mInvalidateInputFallArea != nullptr;
}
