#include "Player/PlayerJudgeWallKeep.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterAfterUpperPunch.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeWallKeep::PlayerJudgeWallKeep(
    const al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
    const IUsePlayerCollision* collider, const IPlayerModelChanger* modelChanger,
    const IUsePlayerHeightCheck* heightCheck,
    const PlayerCounterAfterUpperPunch* counterAfterUpperPunch,
    const PlayerWallActionHistory* wallActionHistory, const PlayerCarryKeeper* carryKeeper,
    const PlayerTrigger* trigger, const PlayerCounterForceRun* counterForceRun)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider),
      mModelChanger(modelChanger), mHeightCheck(heightCheck),
      mCounterAfterUpperPunch(counterAfterUpperPunch), mWallActionHistory(wallActionHistory),
      mCarryKeeper(carryKeeper), mTrigger(trigger), mCounterForceRun(counterForceRun) {}

void PlayerJudgeWallKeep::reset() {
    mIsJudge = false;
}

void PlayerJudgeWallKeep::update() {
    mIsJudge = false;
    if (mModelChanger->is2DModel() || mCounterForceRun->isForceRun() ||
        mCounterAfterUpperPunch->getCounter() < (u32)mConst->getWallInhibitAfterPunch() ||
        (rs::isAboveGround(mHeightCheck) &&
         rs::getGroundHeight(mHeightCheck) < mConst->getWallHeightLowLimit()) ||
        rs::isCollidedGround(mCollider) || !rs::isCollidedWall(mCollider) ||
        rs::isActionCodeNoWallKeepWall(mCollider) || !mCarryKeeper->isCarryWallKeep())
        return;

    const sead::Vector3f& collidedWallNormal = rs::getCollidedWallNormal(mCollider);
    if (!rs::judgeEnableWallKeepHistory(mPlayer, mWallActionHistory, al::getTrans(mPlayer),
                                        collidedWallNormal, mConst->getTall(), true))
        return;

    // required to be explicit block for stack management
    {
        const PlayerInput* input = mInput;
        const al::LiveActor* player = mPlayer;
        f32 wallKeepDegree = mConst->getWallKeepDegree();
        const PlayerTrigger* trigger = mTrigger;
        sead::Vector3f velocity = al::getVelocity(player);
        const sead::Vector3f* gravity = &al::getGravity(player);
        if (!al::tryNormalizeOrZero(&velocity) || velocity.dot(*gravity) <= 0.0f)
            return;

        if (trigger->isOn(PlayerTrigger::EActionTrigger_val30)) {
            if (!input->isMoveDeepDown())
                return;
            sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
            input->calcMoveInput(&moveInput, -*gravity);
            al::normalize(&moveInput);
            if ((-moveInput).dot(collidedWallNormal) <
                sead::Mathf::cos(sead::Mathf::deg2rad(wallKeepDegree)))
                return;
        } else {
            sead::Vector3f facingDir;
            al::calcFrontDir(&facingDir, player);
            f32 cosOfDegree = sead::Mathf::cos(sead::Mathf::deg2rad(wallKeepDegree));
            if ((-facingDir).dot(collidedWallNormal) < cosOfDegree)
                return;

            sead::Vector3f moveDirection = {0.0f, 0.0f, 0.0f};
            input->calcMoveDirection(&moveDirection, -*gravity);
            f32 dot = collidedWallNormal.dot(moveDirection);
            if (dot > 0.0f && dot < cosOfDegree)
                return;
        }

        sead::Vector3f verticalVelocity = {0.0f, 0.0f, 0.0f};
        al::verticalizeVec(&verticalVelocity, *gravity, al::getVelocity(player));
        if (al::isNearZero(verticalVelocity, 0.001f))
            return;

        sead::Vector3f playerUp;
        al::calcUpDir(&playerUp, player);
        sead::Vector3f trans = al::getTrans(player) + playerUp * 120.0f;
        if (!alCollisionUtil::checkStrikeArrow(player, trans, collidedWallNormal * -77.0f, nullptr,
                                               nullptr))
            return;
    }

    if (rs::isCollidedWallFace(mCollider)) {
        mIsJudge = true;
        return;
    }

    // required to be explicit block to match function return pattern
    {
        const sead::Vector3f& gravity2 = al::getGravity(mPlayer);
        sead::Vector3f nearWallPosition =
            rs::getCollidedWallPos(mCollider) - collidedWallNormal * 5.0f;
        sead::Vector3f wallSide = {0.0f, 0.0f, 0.0f};
        wallSide.setCross(collidedWallNormal, gravity2);
        al::normalize(&wallSide);

        sead::Vector3f sideOffset = wallSide * 50.0f;
        al::TriangleFilterWallOnly filterWallOnly = {gravity2};

        const al::ArrowHitInfo* hitInfo1 = nullptr;
        if (alCollisionUtil::getLastPolyOnArrow(mPlayer, &hitInfo1, nearWallPosition + sideOffset,
                                                -sideOffset, nullptr, &filterWallOnly)) {
            const sead::Vector3f& hitPos =
                alCollisionUtil::getCollisionHitPos(hitInfo1->hitInfo.data());
            const sead::Vector3f& hitNormal =
                alCollisionUtil::getCollisionHitNormal(hitInfo1->hitInfo.data());
            if (!rs::calcExistCollisionBorder(mPlayer, hitPos, hitNormal)) {
                if (sead::Mathf::abs(al::calcAngleOnPlaneDegree(
                        hitNormal, collidedWallNormal, gravity2)) > mConst->getWallFollowAngleH())
                    return;
                if (sead::Mathf::abs(al::calcAngleOnPlaneDegree(
                        hitNormal, collidedWallNormal, wallSide)) > mConst->getWallFollowAngleV())
                    return;
            }
        }

        const al::ArrowHitInfo* hitInfo2 = nullptr;
        if (alCollisionUtil::getLastPolyOnArrow(mPlayer, &hitInfo2, nearWallPosition - sideOffset,
                                                sideOffset, nullptr, &filterWallOnly)) {
            const sead::Vector3f& hitPos =
                alCollisionUtil::getCollisionHitPos(hitInfo2->hitInfo.data());
            const sead::Vector3f& hitNormal =
                alCollisionUtil::getCollisionHitNormal(hitInfo2->hitInfo.data());
            if (!rs::calcExistCollisionBorder(mPlayer, hitPos, hitNormal)) {
                if (sead::Mathf::abs(al::calcAngleOnPlaneDegree(
                        hitNormal, collidedWallNormal, gravity2)) > mConst->getWallFollowAngleH())
                    return;
                if (sead::Mathf::abs(al::calcAngleOnPlaneDegree(
                        hitNormal, collidedWallNormal, wallSide)) > mConst->getWallFollowAngleV())
                    return;
            }
        }
    }

    mIsJudge = true;
}

bool PlayerJudgeWallKeep::judge() const {
    return mIsJudge;
}
