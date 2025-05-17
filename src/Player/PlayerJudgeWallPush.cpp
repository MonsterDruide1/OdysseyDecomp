#include "Player/PlayerJudgeWallPush.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

PlayerJudgeWallPush::PlayerJudgeWallPush(const al::LiveActor* player,
                                         const IUsePlayerCollision* collider,
                                         const PlayerInput* input)
    : mPlayer(player), mCollider(collider), mInput(input) {}

void PlayerJudgeWallPush::reset() {}

void PlayerJudgeWallPush::update() {}

bool PlayerJudgeWallPush::judge() const {
    if (rs::isPlayerCarrySomething(mPlayer) || !rs::isCollidedGround(mCollider) ||
        !rs::isCollidedWall(mCollider) || rs::isActionCodeNoActionWall(mCollider))
        return false;

    const sead::Vector3f& collidedGroundNormal = rs::getCollidedGroundNormal(mCollider);
    const sead::Vector3f& collidedWallNormal = rs::getCollidedWallNormal(mCollider);

    if (sead::Mathf::abs(collidedGroundNormal.dot(collidedWallNormal)) > 0.17365f)  // cos(80°)
        return false;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mPlayer);
    al::verticalizeVec(&front, collidedGroundNormal, front);
    if (!al::tryNormalizeOrZero(&front))
        return false;

    if (front.dot(-collidedWallNormal) < 0.70711f || !mInput->isMove())  // cos(45°)
        return false;

    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    al::calcUpDir(&up, mPlayer);
    sead::Vector3f moveDirection = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveDirection(&moveDirection, up);
    return !(moveDirection.dot(front) < 0.70711f);  // cos(45°)
}
