#include "Player/PlayerJudgeWallHitDown.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Player/PlayerTrigger.h"
#include "Util/PlayerCollisionUtil.h"
#include "Stuff.h"
#include "PlayerUtil.h"

PlayerJudgeWallHitDown::PlayerJudgeWallHitDown(const al::LiveActor* player,
                                               const IUsePlayerCollision* collider,
                                               const PlayerConst* pConst,
                                               const PlayerTrigger* trigger)
    : mPlayer(player), mCollider(collider), mConst(pConst), mTrigger(trigger) {}

void PlayerJudgeWallHitDown::reset() {}

void PlayerJudgeWallHitDown::update() {}

bool PlayerJudgeWallHitDown::judge() const {
    if(!rs::isCollidedWall(mCollider)) return false;
    if(rs::isActionCodeNoActionWall(mCollider)) return false;
    if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val9)) return false;

    sead::Vector3f a3 = rs::getCollidedWallNormal(mCollider);
    al::verticalizeVec(&a3, al::getGravity(mPlayer), a3);
    if(!al::tryNormalizeOrZero(&a3)) return false;

    sead::Vector3f v7 = {0.0f, 0.0f, 0.0f};
    if(!rs::calcAlongSkyFront(&v7, mPlayer)) return false;

    return al::calcAngleDegree(a3, -v7) <= mConst->getCollisionHitDownAngleH();
}
