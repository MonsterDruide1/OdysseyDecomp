#include "Player/PlayerExternalVelocity.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Util/AreaUtil.h"
#include "Util/ExternalForceKeeper.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

PlayerExternalVelocity::PlayerExternalVelocity(const al::LiveActor* actor,
                                               const IUsePlayerCollision* collision,
                                               const sead::Vector3f* trans)
    : mActor(actor), mCollision(collision), mTrans(trans), mAreaDrag(mExternalDrag) {
    mExternalForceKeeper = new ExternalForceKeeper();
}

bool PlayerExternalVelocity::receiveMsgPlayer(const al::SensorMsg* msg, al::HitSensor* self,
                                              al::HitSensor* other) {
    sead::Vector3f snapForce = {0.0f, 0.0f, 0.0f};
    if (rs::tryGetObjSnapForce(&snapForce, msg)) {
        mSnapForce = snapForce;
        return true;
    }

    return receiveMsgCap(msg, self, other);
}

bool PlayerExternalVelocity::receiveMsgCap(const al::SensorMsg* msg, al::HitSensor* self,
                                           al::HitSensor* other) {
    return mExternalForceKeeper->receiveMsg(msg, self, other);
}

void PlayerExternalVelocity::requestApplyLastGroundInertia() {
    mApplyLastGroundInertiaFrames = 1;
}

void PlayerExternalVelocity::cancelAndFeedbackLastGroundInertia(al::LiveActor* actor, f32 strength,
                                                                bool isFrontOnly) {
    sead::Vector3f inertia = mLastGroundInertia;
    mTotalVelocity -= mLastGroundInertia;
    mLastGroundInertia = {0.0f, 0.0f, 0.0f};
    mApplyLastGroundInertiaFrames = 0;

    al::verticalizeVec(&inertia, al::getGravity(mActor), inertia);

    sead::Vector3f inertiaHDir = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&inertiaHDir, inertia))
        return;

    if (isFrontOnly) {
        sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&frontDir, mActor);
        al::verticalizeVec(&frontDir, al::getGravity(mActor), frontDir);

        if (al::tryNormalizeOrZero(&frontDir))
            strength *= sead::Mathf::clampMin(frontDir.dot(inertiaHDir), 0.0f);
    }

    al::addVelocity(actor, strength * inertia);
}

void PlayerExternalVelocity::update() {
    mTotalVelocity = {0.0f, 0.0f, 0.0f};

    bool isOnGround = false;
    if (mCollision)
        isOnGround = rs::isOnGround(mActor, mCollision);
    else
        isOnGround = al::isExistActorCollider(mActor) && al::isOnGround(mActor, 0);

    f32 sensitivity = mSensitivity.getValue(isOnGround);
    f32 drag = mExternalDrag.getValue(isOnGround);

    mExternalForceKeeper->calcForce(&mExternalForce);
    mExternalForceKeeper->reset();

    mExternalVelocity *= drag;
    mExternalVelocity += mExternalForce * sensitivity;

    if (mExternalVelocity.length() < 0.1f)
        mExternalVelocity = {0.0f, 0.0f, 0.0f};

    updatePadRumbleExternalForce();

    mAreaVelocity *= mAreaDrag.getValue(isOnGround);

    sead::Vector3f velocity = {0.0f, 0.0f, 0.0f};
    rs::calcExtForceAreaVelocity(&velocity, mActor, al::getTrans(mActor), mAreaVelocity,
                                 al::getVelocity(mActor));

    mAreaVelocity += velocity;
    if (mAreaVelocity.length() < 0.1f)
        mAreaVelocity = {0.0f, 0.0f, 0.0f};

    updateLastGroundForce(isOnGround);

    mTotalVelocity.set(mLastGroundInertia);
    mTotalVelocity += mExternalVelocity;
    mTotalVelocity += mAreaVelocity;
}

void PlayerExternalVelocity::updatePadRumbleExternalForce() {
    if (!mTrans)
        return;

    f32 distance = mExternalVelocity.length();
    if (al::isNearZero(distance)) {
        if (mIsRumbleActive) {
            mIsRumbleActive = false;
            alPadRumbleFunction::stopPadRumbleLoop(mActor, "【ループ】ジー（強）", mTrans);
        }
        return;
    }

    distance = al::easeIn(al::calcRate01(distance, 0.0f, 50.0f));
    if (!mIsRumbleActive) {
        mIsRumbleActive = true;
        alPadRumbleFunction::startPadRumbleLoopNo3D(mActor, "【ループ】ジー（強）", mTrans);
    }

    f32 volume = distance * 0.35f;
    f32 pitch = sead::Mathf::clampMin(distance * -0.25f + 1.0f, 0.1f);

    alPadRumbleFunction::changePadRumbleLoopVolmue(mActor, "【ループ】ジー（強）", mTrans, volume,
                                                   volume);
    alPadRumbleFunction::changePadRumbleLoopPitch(mActor, "【ループ】ジー（強）", mTrans, pitch,
                                                  pitch);
}

void PlayerExternalVelocity::updateLastGroundForce(bool isOnGround) {
    if (!mCollision)
        return;

    mLastGroundInertia = {0.0f, 0.0f, 0.0f};
    if (isOnGround) {
        al::calcForceMovePowerExceptNormal(
            &mMovePowerForce, rs::getCollidedGroundCollisionParts(mCollision),
            rs::getCollidedGroundPos(mCollision), rs::getCollidedGroundNormal(mCollision));
        return;
    }

    if (mApplyLastGroundInertiaFrames < 1)
        mMovePowerForce = {0.0f, 0.0f, 0.0f};
    else
        mLastGroundInertia.set(mMovePowerForce);

    mApplyLastGroundInertiaFrames = al::converge(mApplyLastGroundInertiaFrames, 0, 1);
}

void PlayerExternalVelocity::reset() {
    mExternalForceKeeper->reset();
    mExternalForce = {0.0f, 0.0f, 0.0f};
    mSnapForce = {0.0f, 0.0f, 0.0f};
    mAreaVelocity = {0.0f, 0.0f, 0.0f};
    mTotalVelocity = {0.0f, 0.0f, 0.0f};
    mExternalVelocity = {0.0f, 0.0f, 0.0f};
}

bool PlayerExternalVelocity::isExistForce() const {
    return !al::isNearZero(mAreaVelocity) || !al::isNearZero(mExternalForce) ||
           !al::isNearZero(mExternalVelocity);
}

bool PlayerExternalVelocity::isExistSnapForce() const {
    return !al::isNearZero(mSnapForce);
}
