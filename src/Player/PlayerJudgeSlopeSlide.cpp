#include "Player/PlayerJudgeSlopeSlide.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeSlopeSlide::PlayerJudgeSlopeSlide(const al::LiveActor* player, const PlayerConst* pConst,
                                             const PlayerInput* input,
                                             const IUsePlayerCollision* collider,
                                             const IPlayerModelChanger* modelChanger)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider),
      mModelChanger(modelChanger) {}

void PlayerJudgeSlopeSlide::reset() {
    mIsJudge = false;
}

void PlayerJudgeSlopeSlide::update() {
    mIsJudge = false;
    if (!mModelChanger->is2DModel() && mInput->isHoldHipDrop() &&
        rs::isOnGroundSlopeSlideStart(mPlayer, mCollider, mConst))
        mIsJudge = true;
}

bool PlayerJudgeSlopeSlide::judge() const {
    return mIsJudge;
}
