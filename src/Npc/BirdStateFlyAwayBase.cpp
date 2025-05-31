#include "Npc/BirdStateFlyAwayBase.h"

#include "Library/Placement/PlacementFunction.h"

BirdStateFlyAwayBase::BirdStateFlyAwayBase(const char* name, al::LiveActor* actor)
    : al::ActorStateBase(name, actor) {}

void BirdStateFlyAwayBase::initByPlacementInfo(const al::ActorInitInfo& info) {
    if (al::isExistLinkChild(info, "TargetPos", 0)) {
        mHasTargetPos = true;
        al::getChildTrans(&mTargetPos, info, "TargetPos");
    }
}

void BirdStateFlyAwayBase::kill() {
    al::NerveStateBase::kill();
    mCause = FlyAwayCause::None;
}

void BirdStateFlyAwayBase::startFlyAwayByHomeLanding() {
    mCause = FlyAwayCause::HomeLanding;
}

void BirdStateFlyAwayBase::startFlyAwayByCapGround() {
    mCause = FlyAwayCause::CapGround;
}

void BirdStateFlyAwayBase::startFlyAwayByTouchAir() {
    mCause = FlyAwayCause::TouchAir;
}

void BirdStateFlyAwayBase::startFlyAwayByKoopa() {
    mCause = FlyAwayCause::Koopa;
}

bool BirdStateFlyAwayBase::isFlyAwayByHomeLanding() const {
    return mCause == FlyAwayCause::HomeLanding;
}

bool BirdStateFlyAwayBase::isFlyAwayByCapGround() const {
    return mCause == FlyAwayCause::CapGround;
}

bool BirdStateFlyAwayBase::isFlyAwayByTouchAir() const {
    return mCause == FlyAwayCause::TouchAir;
}

bool BirdStateFlyAwayBase::isFlyAwayByKoopa() const {
    return mCause == FlyAwayCause::Koopa;
}
