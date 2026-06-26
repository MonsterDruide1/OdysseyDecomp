#include "Player/HackCapAboveGroundChecker.h"

#include "Player/IUsePlayerCollision.h"

HackCapAboveGroundChecker::HackCapAboveGroundChecker(const al::LiveActor* actor,
                                                     const IUsePlayerCollision* collision)
    : mActor(actor), mCollision(collision), mIsAboveGround(false) {}

void HackCapAboveGroundChecker::update() {
    // NON_MATCHING
}

bool HackCapAboveGroundChecker::isAboveGround() const {
    return mIsAboveGround;
}
