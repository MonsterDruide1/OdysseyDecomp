#include "Player/PlayerJudgeWallHitDownRolling.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Player/PlayerTrigger.h"
#include "Util/PlayerCollisionUtil.h"
#include "Stuff.h"
#include "PlayerUtil.h"

PlayerJudgeWallHitDownRolling::PlayerJudgeWallHitDownRolling(const al::LiveActor* player,
                                               const IUsePlayerCollision* collider,
                                               const PlayerConst* pConst,
                                               const PlayerTrigger* trigger)
    : mPlayer(player), mCollider(collider), mConst(pConst), mTrigger(trigger) {}

void PlayerJudgeWallHitDownRolling::reset() {}

void PlayerJudgeWallHitDownRolling::update() {}

bool PlayerJudgeWallHitDownRolling::judge() const {
    if(!rs::isCollidedWall(mCollider)) return false;
    if(rs::isActionCodeNoActionWall(mCollider)) return false;
    if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val9)) return false;

    sead::Vector3f gravity = al::getGravity(mPlayer);
    sead::Vector3f a3 = rs::getCollidedWallNormal(mCollider);
    al::verticalizeVec(&a3, gravity, a3);
    if(!al::tryNormalizeOrZero(&a3)) return false;

    sead::Vector3f v7 = {0.0f, 0.0f, 0.0f};
    if(!rs::calcAlongSkyFront(&v7, mPlayer)) return false;

    if(!(al::calcAngleDegree(a3, -v7) <= mConst->getCollisionHitDownAngleH())) return false;

    sead::Vector3f v15 = {0.0f, 0.0f, 0.0f};
    if(!rs::isCollidedGround(mCollider)) return true;

    sead::Vector3f collidedWallPos = rs::getCollidedWallPos(mCollider);
    f32 v12 = (rs::getCollidedGroundPos(mCollider) - collidedWallPos).dot(gravity);
    if(al::isNearZeroOrLess(v12 - mConst->getCollisionSmallStepHeight(), 0.001f)) return false;

    v15 = rs::getCollidedGroundNormal(mCollider);
    return al::calcAngleDegree(v15, a3) > mConst->getCollisionHitDownEscapeAngleV();
}
