#include "Player/PlayerRecoverySafetyPoint.h"

#include "Library/LiveActor/ActorActionFunction.h"

PlayerRecoverySafetyPoint::PlayerRecoverySafetyPoint(const al::LiveActor* actor,
                                                     const HackCap* hackCap,
                                                     const al::ActorInitInfo* initInfo,
                                                     const IUseDimension* dimension,
                                                     al::CollisionPartsFilterBase* colFilter,
                                                     al::HitSensor* hitSensor)
    : mActor(actor), mHackCap(hackCap), mDimension(dimension), mColFilter(colFilter),
      mHitSensor(hitSensor), mBubble3D(nullptr), mBubble2D(nullptr), mRecoveryTimer(0),
      mHasSafety3D(false), mSafetyPos3D(0, 0, 0), mSafetyNormal3D(0, 0, 0), mGravity3D(0, 0, 0),
      mArea3D(nullptr), mHasSafety2D(false), mSafetyPos2D(0, 0, 0), mSafetyNormal2D(0, 0, 0),
      mGravity2D(0, 0, 0), mArea2D(nullptr), mRecoveryArea(nullptr) {
    mBubble3D = new al::LiveActor("TractorBubble");
    al::initChildActorWithArchiveNameNoPlacementInfo(mBubble3D, *initInfo, "TractorBubble",
                                                     nullptr);
    al::invalidateClipping(mBubble3D);

    mBubble2D = new al::LiveActor("TractorBubble2D");
    al::initChildActorWithArchiveNameNoPlacementInfo(mBubble2D, *initInfo, "TractorBubble2D",
                                                     nullptr);
    al::invalidateClipping(mBubble2D);
}

const al::AreaObj* PlayerRecoverySafetyPoint::getSafetyPointArea() const {
    return !(rs::isPlayer2D(mActor)) ? mArea3D : mArea2D;
}

const sead::Vector3f* PlayerRecoverySafetyPoint::getSafetyPointGravity() const {
    return !(rs::isPlayer2D(mActor)) ? &mGravity3D : &mGravity2D;
}

void PlayerRecoverySafetyPoint::setRecoveryArea(const al::AreaObj* area) {
    mRecoveryArea = area;
}

bool PlayerRecoverySafetyPoint::isActiveRecoveryArea() const {
    return mRecoveryArea && mRecoveryArea->isValid();
}

bool PlayerRecoverySafetyPoint::isEnableRecovery() const {
    if (rs::isKidsMode(mActor) || (mRecoveryArea && mRecoveryArea->isValid()))
        return true;
    return mHackCap->isEnableRescuePlayer();
}

bool PlayerRecoverySafetyPoint::isValid() const {
    if (rs::isKidsMode(mActor) || (mRecoveryArea && mRecoveryArea->isValid()) ||
        mHackCap->isEnableRescuePlayer()) {
        if (rs::isPlayer2D(mActor))
            return mHasSafety2D;
        return mHasSafety3D;
    }
    return false;
}

void PlayerRecoverySafetyPoint::startBubbleWait() {
    if (!rs::isPlayer2D(mActor))
        al::startAction(mBubble3D, "Wait");
    return;
}

void PlayerRecoverySafetyPoint::endRecovery() {
    if (mIsRecovering) {
        if (rs::isPlayer2D(mActor))
            mBubble2D->kill();
        else
            al::startAction(mBubble3D, "Disappear");

        mIsRecovering = false;
    }
}

void PlayerRecoverySafetyPoint::updateRecoveryAreaValidity() {
    if (mRecoveryArea && !mRecoveryArea->isValid())
        mRecoveryArea = nullptr;
    return;
}
