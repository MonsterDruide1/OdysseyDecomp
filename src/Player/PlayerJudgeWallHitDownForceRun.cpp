#include "Player/PlayerJudgeWallHitDownForceRun.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/PlayerCollisionUtil.h"

bool sub_710045CA18(const PlayerConst* x0_0, const sead::Vector3f& a2, const sead::Vector3f& a3,
                    const sead::Vector3f& a4, const sead::Vector3f& a5) {

    sead::Vector3f a1 = {0.0, 0.0, 0.0};
    al::verticalizeVec(&a1, a4, a3);
    if (!al::tryNormalizeOrZero(&a1))
        return false;

    sead::Vector3f v18 = -a2;
    if (!(al::calcAngleDegree(a1, v18) <= x0_0->getCollisionHitDownAngleH()))
        return false;

    f32 hitDownEscapeAngleV = x0_0->getCollisionHitDownEscapeAngleV();
    return al::isInRange(al::calcAngleDegree(a5, a3), hitDownEscapeAngleV, 180.0 - hitDownEscapeAngleV);
}

PlayerJudgeWallHitDownForceRun::PlayerJudgeWallHitDownForceRun(
    const al::LiveActor* player, const IUsePlayerCollision* collider, const PlayerConst* pConst,
    const PlayerCounterForceRun* counterForceRun, const PlayerTrigger* trigger)
    : mPlayer(player), mCollider(collider), mConst(pConst), mCounterForceRun(counterForceRun),
      mTrigger(trigger) {}

void PlayerJudgeWallHitDownForceRun::reset() {}

void PlayerJudgeWallHitDownForceRun::update() {}

bool PlayerJudgeWallHitDownForceRun::judge() const {
    if (!mCounterForceRun->isForceRun())
        return false;
    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val9))
        return false;

    bool v2 = rs::isCollidedWall(mCollider) ? !rs::isActionCodeNoActionWall(mCollider) : false;
    bool v3 =
        rs::isCollidedCeiling(mCollider) ? !rs::isActionCodeNoActionCeiling(mCollider) : false;
    if ((v2 || v3) == 0)
        return false;

    sead::Vector3f v7 = {0.0f, 0.0f, 0.0f};
    if (!rs::calcAlongSkyFront(&v7, mPlayer))
        return false;

    sead::Vector3f v12 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&v12, mPlayer, mCollider);
    sead::Vector3f v11 = -al::getGravity(mPlayer);

    if (v2 && sub_710045CA18(mConst, v7, rs::getCollidedWallNormal(mCollider), v11, v12))
        return true;

    if (v3 && sub_710045CA18(mConst, v7, rs::getCollidedCeilingNormal(mCollider), v11, v12))
        return true;

    return false;
}
