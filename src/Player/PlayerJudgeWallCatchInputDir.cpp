#include "Player/PlayerJudgeWallCatchInputDir.h"

#include <math/seadQuat.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/IUsePlayerHeightCheck.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerExternalVelocity.h"
#include "Player/PlayerInput.h"
#include "System/GameDataUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeWallCatchInputDir::PlayerJudgeWallCatchInputDir(
    const al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
    const IUsePlayerCollision* collision, const IPlayerModelChanger* modelChanger,
    const PlayerCarryKeeper* carryKeeper, const PlayerExternalVelocity* externalVelocity,
    const PlayerCounterForceRun* counterForceRun, const IUsePlayerHeightCheck* heightCheck)
    : mPlayer(player), mConst(pConst), mInput(input), mCollision(collision),
      mModelChanger(modelChanger), mCarryKeeper(carryKeeper), mExternalVelocity(externalVelocity),
      mCounterForceRun(counterForceRun), mHeightCheck(heightCheck) {
    sead::Quatf quat = sead::Quatf::unit;
    al::calcQuat(&quat, player);
    mCollisionPartsConnector = al::createCollisionPartsConnector(player, quat);
}

void PlayerJudgeWallCatchInputDir::reset() {
    mIsJudge = false;
    mIsOnGroundSkateCode = false;
    mHitPos = {0.0f, 0.0f, 0.0f};
    mHitNormal = {0.0f, 0.0f, 0.0f};
    mHitNormal2 = {0.0f, 0.0f, 0.0f};
    al::disconnectMtxConnector(mCollisionPartsConnector);
    mGroundNormal = {0.0f, 0.0f, 0.0f};
    mTimeInAir = 0;
}

void PlayerJudgeWallCatchInputDir::update() {
    mIsJudge = false;

    if (mCarryKeeper->isCarry())
        return;
    if (mModelChanger->is2DModel())
        return;
    if (mCounterForceRun->isForceRun())
        return;
    if (mExternalVelocity->isExistForce())
        return;
    if (rs::isCollidedGround(mCollision))
        return;
    if (mHeightCheck->isAboveGround() && mHeightCheck->getGroundHeight() <= 20.0f)
        return;

    if (mIsKidsMode)
        mIsJudge = updateJudgeKids();
    else
        mIsJudge = updateJudgeNormal();
}

bool PlayerJudgeWallCatchInputDir::updateJudgeKids() {
    if (updateJudgeNormal())
        return true;

    if (!al::isMtxConnectorConnecting(mCollisionPartsConnector))
        return false;
    if (mTimeInAir < 5)
        return false;
    if (mHeightCheck->isAboveGround() && mHeightCheck->getGroundHeight() < 600.0f)
        return false;

    sead::Vector3f connectTrans = {0.0f, 0.0f, 0.0f};
    al::calcConnectTrans(&connectTrans, mCollisionPartsConnector);
    sead::Vector3f verticalDiff = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&verticalDiff, al::getGravity(mPlayer),
                       connectTrans - al::getTrans(mPlayer));
    if (verticalDiff.length() > 200.0f)
        return false;

    sead::Vector3f verticalDir = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&verticalDir, verticalDiff))
        return false;

    return rs::findWallCatchPosWallHit(
        &mCollisionParts, &mHitPos, &mHitNormal2, &mHitNormal, mPlayer, verticalDir,
        al::getTrans(mPlayer), 200.0f, 90.0f, mConst->getWallCatchDegree(),
        mConst->getWallCatchHeightEdgeTop(), 0.0f, mConst->getWallCatchHeightBottom(),
        mConst->getCollisionRadius(), mConst->getCollisionRadiusStand());
}

bool PlayerJudgeWallCatchInputDir::updateJudgeNormal() {
    sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveDirection(&moveDir, -al::getGravity(mPlayer));
    if (al::isNearZero(moveDir)) {
        if (rs::isCollidedWall(mCollision))
            moveDir.set(rs::getCollidedWallNormal(mCollision));
        else if (mIsOnGroundSkateCode)
            return false;
        else
            al::calcFrontDir(&moveDir, mPlayer);

        moveDir.negate();
    }

    f32 moveSpeed = 75.0f;
    if (mIsOnGroundSkateCode)
        moveSpeed += sead::Mathf::clampMax(moveDir.dot(al::getVelocity(mPlayer)), 0.0f);

    return rs::findWallCatchPosWallHit(
        &mCollisionParts, &mHitPos, &mHitNormal2, &mHitNormal, mPlayer, moveDir,
        al::getTrans(mPlayer), moveSpeed, mConst->getWallKeepDegree(), mConst->getWallCatchDegree(),
        mConst->getWallCatchHeightEdgeTop(), 0.0f, mConst->getWallCatchHeightBottom(),
        mConst->getCollisionRadius(), mConst->getCollisionRadiusStand());
}

bool PlayerJudgeWallCatchInputDir::judge() const {
    return mIsJudge;
}

void PlayerJudgeWallCatchInputDir::validateFallJudge() {
    mValidTimer = 10;
}

void PlayerJudgeWallCatchInputDir::updateWallCatchEnviroment() {
    mIsKidsMode = rs::isKidsMode(mPlayer);
    mValidTimer = al::converge(mValidTimer, 0, 1);

    if (!rs::isCollidedGround(mCollision)) {
        mTimeInAir = al::converge(mTimeInAir, 5, 1);
        return;
    }

    mIsOnGroundSkateCode = rs::isOnGroundSkateCode(mPlayer, mCollision);
    al::attachMtxConnectorToCollisionParts(mCollisionPartsConnector,
                                           rs::getCollidedGroundCollisionParts(mCollision));

    sead::Quatf quat = sead::Quatf::unit;
    al::calcQuat(&quat, mPlayer);
    al::setConnectorBaseQuatTrans(quat, rs::getCollidedGroundPos(mCollision),
                                  mCollisionPartsConnector);
    mGroundNormal.set(rs::getCollidedGroundNormal(mCollision));
    mTimeInAir = 0;
}
