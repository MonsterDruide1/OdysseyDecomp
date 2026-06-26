#include "Player/PlayerEyeSensorHitHolder.h"

PlayerEyeSensorHitHolder::PlayerEyeSensorHitHolder(al::LiveActor* actor)
    : mActor(actor), mIsHit(false), mHitPos(sead::Vector3f::zero) {}

void PlayerEyeSensorHitHolder::update() {
    // NON_MATCHING
}

bool PlayerEyeSensorHitHolder::isHit() const {
    return mIsHit;
}

const sead::Vector3f& PlayerEyeSensorHitHolder::getHitPos() const {
    return mHitPos;
}
