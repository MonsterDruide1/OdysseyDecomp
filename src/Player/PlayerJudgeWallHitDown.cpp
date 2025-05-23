#include "Player/PlayerJudgeWallHitDown.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeWallHitDown::PlayerJudgeWallHitDown(const al::LiveActor* player,
                                               const IUsePlayerCollision* collider,
                                               const PlayerConst* pConst,
                                               const PlayerTrigger* trigger)
    : mPlayer(player), mCollider(collider), mConst(pConst), mTrigger(trigger) {}

void PlayerJudgeWallHitDown::reset() {}

void PlayerJudgeWallHitDown::update() {}

bool PlayerJudgeWallHitDown::judge() const {
    if (!rs::isCollidedWall(mCollider))
        return false;
    if (rs::isActionCodeNoActionWall(mCollider))
        return false;
    if (mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val9))
        return false;

    const sead::Vector3f& gravity = al::getGravity(mPlayer);
    sead::Vector3f wallNormalH = rs::getCollidedWallNormal(mCollider);
    al::verticalizeVec(&wallNormalH, gravity, wallNormalH);
    if (!al::tryNormalizeOrZero(&wallNormalH))
        return false;

    sead::Vector3f alongSkyFront = {0.0f, 0.0f, 0.0f};
    if (!rs::calcAlongSkyFront(&alongSkyFront, mPlayer))
        return false;

    return al::calcAngleDegree(wallNormalH, -alongSkyFront) <= mConst->getCollisionHitDownAngleH();
}
