#include "Player/PlayerJudgeEnableStandUp.h"

#include "Player/IUsePlayerCeilingCheck.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeEnableStandUp::PlayerJudgeEnableStandUp(const IUsePlayerCollision* collider,
                                                   const IUsePlayerCeilingCheck* ceilingChecker)
    : mCollider(collider), mCeilingChecker(ceilingChecker) {}

bool PlayerJudgeEnableStandUp::judge() const {
    return mCeilingChecker->isEnableStandUp() || !rs::isCollidedGround(mCollider);
}
