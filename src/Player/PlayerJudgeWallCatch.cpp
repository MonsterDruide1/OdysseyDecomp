#include "Player/PlayerJudgeWallCatch.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerExternalVelocity.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeWallCatch::PlayerJudgeWallCatch(const al::LiveActor* player, const PlayerConst* pConst,
                                           const IUsePlayerCollision* collider,
                                           const IPlayerModelChanger* modelChanger,
                                           const PlayerCarryKeeper* carryKeeper,
                                           const PlayerExternalVelocity* externalVelocity,
                                           const PlayerInput* input, const PlayerTrigger* trigger,
                                           const PlayerCounterForceRun* counterForceRun)
    : mPlayer(player), mConst(pConst), mCollision(collider), mModelChanger(modelChanger),
      mCarryKeeper(carryKeeper), mExternalVelocity(externalVelocity), mInput(input),
      mTrigger(trigger), mCounterForceRun(counterForceRun) {}

void PlayerJudgeWallCatch::reset() {
    mIsJudge = false;
    mPosition = {0.0f, 0.0f, 0.0f};
    mCollidedWallNormal = {0.0f, 0.0f, 0.0f};
    mNormalAtPos = {0.0f, 0.0f, 0.0f};
}

void PlayerJudgeWallCatch::update() {
    mIsJudge = false;
    if (mCarryKeeper->isCarry() || mModelChanger->is2DModel() || mCounterForceRun->isForceRun() ||
        mExternalVelocity->isExistForce() || !rs::isCollidedWall(mCollision) ||
        rs::isCollidedGround(mCollision) || rs::isActionCodeNoWallGrab(mCollision))
        return;

    sead::Vector3f facingDir = {0.0f, 0.0f, 0.0f};
    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val30)) {
        if (!mInput->isMoveDeepDown())
            return;
        sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
        mInput->calcMoveInput(&moveDir, -al::getGravity(mPlayer));
        al::normalize(&facingDir, moveDir);
    } else {
        al::calcFrontDir(&facingDir, mPlayer);
    }

    mCollidedWallPart = rs::getCollidedWallCollisionParts(mCollision);
    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    mCollidedWallPart->calcForceMovePower(&movePower, rs::getCollidedWallPos(mCollision));
    sead::Vector3f finalWallPos = rs::getCollidedWallPos(mCollision) + movePower;
    mCollidedWallNormal.set(rs::getCollidedWallNormal(mCollision));

    mIsJudge = rs::findWallCatchPos(
        &mCollidedWallPart, &mPosition, &mNormalAtPos, mPlayer, facingDir, finalWallPos,
        mCollidedWallNormal, mConst->getWallKeepDegree(), mConst->getWallCatchDegree(),
        mConst->getWallCatchHeightEdgeTop(), 0.0f, mConst->getWallCatchHeightBottom(),
        mConst->getCollisionRadius(), mConst->getCollisionRadiusStand());
}

bool PlayerJudgeWallCatch::judge() const {
    return mIsJudge;
}
