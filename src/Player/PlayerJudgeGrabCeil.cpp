#include "Player/PlayerJudgeGrabCeil.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerExternalVelocity.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeGrabCeil::PlayerJudgeGrabCeil(const al::LiveActor* player, const PlayerConst* pConst,
                                         const IUsePlayerCollision* collider,
                                         const IPlayerModelChanger* modelChanger,
                                         const PlayerCarryKeeper* carryKeeper,
                                         const PlayerExternalVelocity* externalVelocity)
    : mPlayer(player), mConst(pConst), mCollision(collider), mModelChanger(modelChanger),
      mCarryKeeper(carryKeeper), mExternalVelocity(externalVelocity) {}

void PlayerJudgeGrabCeil::reset() {
    mIsJudge = false;
    _48 = {0.0f, 0.0f, 0.0f};
    _54 = {0.0f, 0.0f, 0.0f};
    _60 = {0.0f, 0.0f, 0.0f};
}

void PlayerJudgeGrabCeil::update() {
    mIsJudge = false;

    if (mCarryKeeper->isCarry() || mModelChanger->is2DModel() ||
        mExternalVelocity->isExistForce() || !rs::isCollisionCodeGrabCeilAny(mCollision))
        return;

    if (rs::isCollisionCodeGrabCeilWall(mCollision)) {
        sead::Vector3f pos = rs::getCollidedWallPos(mCollision);
        sead::Vector3f normal = rs::getCollidedWallNormal(mCollision);

        if (!mExternalVelocity->isExistSnapForce()) {
            sead::Vector3f verticalVelocity = al::getVelocity(mPlayer);
            al::verticalizeVec(&verticalVelocity, al::getGravity(mPlayer), verticalVelocity);
            if (al::tryNormalizeOrZero(&verticalVelocity) && normal.dot(verticalVelocity) > 0.0f)
                return;
        }

        sead::Vector3f forceMovePower = {0.0f, 0.0f, 0.0f};
        rs::getCollidedWallCollisionParts(mCollision)->calcForceMovePower(&forceMovePower, pos);

        mIsJudge = rs::findGrabCeilPosWallHit(
            &mCollidedParts, &_48, &_54, &_60, mPlayer, -normal, pos + forceMovePower,
            mConst->getTall(), mConst->getGrabCeilRange(), mConst->getGrabCeilBodyRadius());

        return;
    }

    sead::Vector3f collidedNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f collidedPos = {0.0f, 0.0f, 0.0f};
    const al::CollisionParts* collidedParts = nullptr;

    if (rs::isCollisionCodeGrabCeilCeiling(mCollision)) {
        collidedNormal = rs::getCollidedCeilingNormal(mCollision);
        collidedPos = rs::getCollidedCeilingPos(mCollision);
        collidedParts = rs::getCollidedCeilingCollisionParts(mCollision);
    } else if (rs::isCollisionCodeGrabCeilGrround(mCollision)) {
        collidedNormal = rs::getCollidedGroundNormal(mCollision);
        collidedPos = rs::getCollidedGroundPos(mCollision);
        collidedParts = rs::getCollidedGroundCollisionParts(mCollision);
        if (!mExternalVelocity->isExistSnapForce()) {
            sead::Vector3f velocity = al::getVelocity(mPlayer);
            if (al::tryNormalizeOrZero(&velocity) && velocity.dot(collidedNormal) > 0.0f)
                return;
        }
    }

    sead::Vector3f forceMovePower = {0.0f, 0.0f, 0.0f};
    collidedParts->calcForceMovePower(&forceMovePower, collidedPos);

    mIsJudge = rs::findGrabCeilPosNoWallHit(
        &mCollidedParts, &_48, &_54, &_60, mPlayer, collidedNormal, collidedPos + forceMovePower,
        mConst->getTall(), mConst->getGrabCeilRange(), mConst->getGrabCeilBodyRadius());

    if (!mIsJudge)
        return;

    f32 dot = _54.dot(al::getVelocity(mPlayer));
    if (sead::Mathf::abs(dot) > mConst->getNormalMinSpeed()) {
        if (dot < 0.0f) {
            _54 *= -1;
            al::normalize(&_54);
        }
    } else {
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mPlayer);
        if (front.dot(_54) < 0.0f) {
            _54 *= -1;
            al::normalize(&_54);
        }
    }
}

bool PlayerJudgeGrabCeil::judge() const {
    return mIsJudge;
}
