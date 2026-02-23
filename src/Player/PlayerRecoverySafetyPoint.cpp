#include "Player/PlayerRecoverySafetyPoint.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"

#include "Player/HackCap.h"
#include "System/GameDataUtil.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

PlayerRecoverySafetyPoint::PlayerRecoverySafetyPoint(const al::LiveActor* actor,
                                                     const HackCap* hackCap,
                                                     const al::ActorInitInfo& initInfo,
                                                     const IUseDimension* dimension,
                                                     al::CollisionPartsFilterBase* colFilter,
                                                     al::HitSensor* hitSensor)
    : mActor(actor), mHackCap(hackCap), mDimension(dimension), mColFilter(colFilter),
      mHitSensor(hitSensor) {
    mTractorBubble = new al::LiveActor("復帰泡");
    al::initChildActorWithArchiveNameNoPlacementInfo(mTractorBubble, initInfo, "TractorBubble",
                                                     nullptr);
    al::invalidateClipping(mTractorBubble);
    mTractorBubble->makeActorDead();

    mTractorBubble2D = new al::LiveActor("復帰泡2D");
    al::initChildActorWithArchiveNameNoPlacementInfo(mTractorBubble2D, initInfo, "TractorBubble2D",
                                                     nullptr);
    al::invalidateClipping(mTractorBubble2D);
    mTractorBubble2D->makeActorDead();
}

void PlayerRecoverySafetyPoint::reset() {
    mSafety3D.reset();
    mSafety2D.reset();

    mRecoveryArea = nullptr;
    mDefaultSafetyPos = nullptr;
}

void PlayerRecoverySafetyPoint::setSafetyPoint(const sead::Vector3f& safetyPos,
                                               const sead::Vector3f& safetyNormal,
                                               const al::AreaObj* areaObj) {
    if (!rs::isPlayer2D(mActor)) {
        mSafety3D.set(safetyPos, safetyNormal, al::getGravity(mActor), areaObj);
    } else if (rs::isIn2DArea(mDimension)) {
        sead::Vector3f snapPos = safetyPos;
        rs::calcSnap2DPosition(&snapPos, mDimension, safetyPos, 500.0f);

        mSafety2D.set(snapPos, safetyNormal, al::getGravity(mActor), areaObj);
    }

    mDefaultSafetyPos = nullptr;
}

void PlayerRecoverySafetyPoint::noticeRequestSafetyPoint(const sead::Vector3f& safetyPos,
                                                         const sead::Vector3f& safetyNormal,
                                                         const al::AreaObj* areaObj) {
    sead::Vector3f pos = safetyPos;
    sead::Vector3f normal = safetyNormal;
    sead::Vector3f* defaultPos = nullptr;

    if (rs::isPlayer3D(mActor) && areaObj == nullptr) {
        const sead::Vector3f& gravity = al::getGravity(mActor);

        sead::Vector3f rayStart = safetyPos - gravity * 10.0f;
        sead::Vector3f rayDir = gravity * 110.0f;
        const al::ArrowHitInfo* hitInfo = nullptr;
        al::TriangleFilterGroundOnly filter(gravity);
        if (!alCollisionUtil::getFirstPolyOnArrow(mActor, &hitInfo, rayStart, rayDir, nullptr,
                                                  &filter))
            return;

        if (!rs::isEnableRecordSafetyPoint(&defaultPos, *hitInfo->hitInfo.data(), mHitSensor,
                                           -gravity))
            return;

        pos = alCollisionUtil::getCollisionHitPos(hitInfo->hitInfo.data());
        normal = alCollisionUtil::getCollisionHitNormal(hitInfo->hitInfo.data());
    }

    setSafetyPoint(pos, normal, areaObj);
    mDefaultSafetyPos = defaultPos;
}

void PlayerRecoverySafetyPoint::noticeDangerousPoint(const sead::Vector3f& pos, bool adjustFlag) {
    if (rs::isPlayer2D(mActor)) {
        if (rs::isIn2DArea(mDimension)) {
            sead::Vector3f snapPos = pos;
            rs::calcSnap2DPosition(&snapPos, mDimension, pos, 500.0f);
            slideLastSafetyPoint(&mSafety2D.safetyPos, &mSafety2D.safetyNormal, mSafety2D.hasSafety,
                                 snapPos, adjustFlag, mColFilter);
        }

        return;
    }

    slideLastSafetyPoint(&mSafety3D.safetyPos, &mSafety3D.safetyNormal, mSafety3D.hasSafety, pos,
                         adjustFlag, nullptr);
    return;
}

void PlayerRecoverySafetyPoint::slideLastSafetyPoint(sead::Vector3f* safetyPos,
                                                     sead::Vector3f* safetyNormal, bool hasSafety,
                                                     const sead::Vector3f& lastSafetyPos,
                                                     bool doValidateRay,
                                                     al::CollisionPartsFilterBase* colFilter) {
    if (!hasSafety)
        return;

    sead::Vector3f slideDir(0.0f, 0.0f, 0.0f);
    al::verticalizeVec(&slideDir, *safetyNormal, *safetyPos - lastSafetyPos);

    f32 length = slideDir.length();
    if (length > 100.0f || !al::tryNormalizeOrZero(&slideDir))
        return;

    const sead::Vector3f& gravity = al::getGravity(mActor);
    sead::Vector3f gravityOffset = 2 * (gravity * 50.0f);
    sead::Vector3f gravityScale = gravity * 50.0f;

    const al::ArrowHitInfo* hitInfo = nullptr;

    if (doValidateRay) {
        sead::Vector3f rayStart = *safetyPos - (length * slideDir) - gravityScale;
        al::TriangleFilterGroundOnly filter(gravity);
        if (alCollisionUtil::getFirstPolyOnArrow(mActor, &hitInfo, rayStart, gravityOffset,
                                                 colFilter, &filter)) {
            sead::Vector3f* outPos = nullptr;
            if (rs::isEnableRecordSafetyPoint(&outPos, *hitInfo->hitInfo.data(), mHitSensor,
                                              -gravity))
                return;
        }
    }

    length = sead::Mathf::clampMin(100.0f - length, 0.0f);
    sead::Vector3f rayStart = *safetyPos + (length * slideDir) - gravityScale;

    sead::Vector3f* outPos = nullptr;
    if (alCollisionUtil::getFirstPolyOnArrow(mActor, &hitInfo, rayStart, gravityOffset, colFilter,
                                             nullptr)) {
        if (rs::isEnableRecordSafetyPoint(&outPos, *hitInfo->hitInfo.data(), mHitSensor,
                                          -gravity) &&
            rs::isCollisionCodeSafetyPoint(*hitInfo->hitInfo.data())) {
            sead::Vector3f hitPos = alCollisionUtil::getCollisionHitPos(hitInfo->hitInfo.data());
            sead::Vector3f hitNormal =
                alCollisionUtil::getCollisionHitNormal(hitInfo->hitInfo.data());

            al::TriangleFilterGroundOnly filter(al::getGravity(mActor));
            if (!alCollisionUtil::getFirstPolyOnArrow(mActor, &hitInfo, *safetyPos - gravity * 5.0f,
                                                      hitPos - *safetyPos, colFilter, &filter)) {
                *safetyPos = hitPos;
                *safetyNormal = hitNormal;
            }
        }
    }
}

bool PlayerRecoverySafetyPoint::isValid() const {
    if (isEnableRecovery()) {
        if (rs::isPlayer2D(mActor))
            return mSafety2D.hasSafety;
        return mSafety3D.hasSafety;
    }
    return false;
}

bool PlayerRecoverySafetyPoint::isEnableRecovery() const {
    return rs::isKidsMode(mActor) || isActiveRecoveryArea() || mHackCap->isEnableRescuePlayer();
}

const sead::Vector3f& PlayerRecoverySafetyPoint::getSafetyPoint() const {
    if (mDefaultSafetyPos != nullptr)
        return *mDefaultSafetyPos;

    if (rs::isPlayer2D(mActor))
        return mSafety2D.safetyPos;
    else
        return mSafety3D.safetyPos;
}

const sead::Vector3f& PlayerRecoverySafetyPoint::getSafetyPointGravity() const {
    if (rs::isPlayer2D(mActor))
        return mSafety2D.gravity;
    return mSafety3D.gravity;
}

const al::AreaObj* PlayerRecoverySafetyPoint::getSafetyPointArea() const {
    if (rs::isPlayer2D(mActor))
        return mSafety2D.area;
    return mSafety3D.area;
}

void PlayerRecoverySafetyPoint::updateRecoveryAreaValidity() {
    if (mRecoveryArea && !mRecoveryArea->isValid())
        mRecoveryArea = nullptr;
}

void PlayerRecoverySafetyPoint::setRecoveryArea(const al::AreaObj* area) {
    mRecoveryArea = area;
}

bool PlayerRecoverySafetyPoint::isActiveRecoveryArea() const {
    return mRecoveryArea && mRecoveryArea->isValid();
}

void PlayerRecoverySafetyPoint::checkInvalidateArea() {
    if (isValid() && al::isInAreaObj(mActor, "InvalidateRecoveryPosArea", getSafetyPoint()))
        reset();
}

void PlayerRecoverySafetyPoint::startRecovery(f32 height) {
    mBubbleHeight = height;  // height will be 80.0 if the player is 3D, else 60.0.
    if (mIsRecovering)
        return;

    mIsRecovering = true;
    if (rs::isPlayer2D(mActor)) {
        mTractorBubble2D->appear();
        return;
    }

    mTractorBubble->appear();
    al::offCollide(mTractorBubble);
    al::startAction(mTractorBubble, "Appear");
    updateRecoveryBubble();

    if (mHackCap->isEnableRescuePlayer()) {
        al::LiveActor* hat = al::getSubActor(mTractorBubble, "装着帽子");
        hat->appear();
        al::startAction(hat, "LockOn");

        al::startHitReactionHitEffect(mHackCap, "プレイヤー救出出現",
                                      al::getTrans(mTractorBubble) -
                                          al::getGravity(mTractorBubble) * 160.0f);

        al::LiveActor* eye = al::getSubActor(mTractorBubble, "目");
        eye->appear();
        al::startAction(eye, "Appear");
    }
}

void PlayerRecoverySafetyPoint::updateRecoveryBubble() {
    if (rs::isPlayer2D(mActor)) {
        if (al::isDead(mTractorBubble2D))
            return;

        sead::Vector3f up = -al::getGravity(mActor);
        sead::Vector3f camFace = sead::Vector3f(0.0f, 0.0f, 0.0f);

        al::calcSideDir(&camFace, mActor);

        sead::Quatf quat = sead::Quatf::unit;

        if (al::isParallelDirection(camFace, up, 0.01f))
            al::calcQuat(&quat, mTractorBubble2D);
        else
            al::makeQuatSideUp(&quat, camFace, up);

        sead::Vector3f upDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
        al::calcUpDir(&upDir, mActor);

        al::resetPosition(mTractorBubble2D, al::getTrans(mActor) + upDir * mBubbleHeight);
        al::updatePoseQuat(mTractorBubble2D, quat);
        return;
    }

    if (al::isDead(mTractorBubble))
        return;

    if (al::isActionPlaying(mTractorBubble, "Disappear")) {
        if (al::isActionEnd(mTractorBubble)) {
            mTractorBubble->kill();
            al::getSubActor(mTractorBubble, "装着帽子")->kill();
            al::getSubActor(mTractorBubble, "目")->kill();
        }
        return;
    }

    sead::Vector3f upDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcUpDir(&upDir, mActor);

    al::resetPosition(mTractorBubble, al::getTrans(mActor) + upDir * mBubbleHeight);

    sead::Vector3f camFace = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcCameraFront(&camFace, mActor, 0);

    sead::Vector3f up = -al::getGravity(mActor);

    if (al::isParallelDirection(camFace, up, 0.01f))
        camFace = al::getCameraUp(mActor, 0);
    camFace.negate();
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatUpFront(&quat, up, camFace);
    al::updatePoseQuat(mTractorBubble, quat);
}

void PlayerRecoverySafetyPoint::startBubbleWait() {
    if (!rs::isPlayer2D(mActor))
        al::startAction(mTractorBubble, "Wait");
}

void PlayerRecoverySafetyPoint::endRecovery() {
    if (mIsRecovering) {
        if (rs::isPlayer2D(mActor))
            mTractorBubble2D->kill();
        else
            al::startAction(mTractorBubble, "Disappear");

        mIsRecovering = false;
    }
}
