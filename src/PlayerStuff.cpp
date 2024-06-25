#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerCollider.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/PlayerCollisionUtil.h"
#include "playerUtil.h"

#include "Player/PlayerColliderHakoniwa.h"


void rs::initPlayerActorInfo(PlayerActorBase* player, PlayerInitInfo const& info) {
    player->mPortNo = info.mPortNo;
    player->mViewMtx = info.mViewMtx;

    sead::Matrix34f mtx;
    mtx.makeQT(info._34, info._20);
    al::updatePoseMtx(player, &mtx);

}

void rs::resetCollisionPose(const IUsePlayerCollision *collider, const sead::Quat<float> &quat) {
    collider->getPlayerCollider()->resetPose(quat);
}

f32 rs::getGroundHeight(const IUsePlayerHeightCheck* heightCheck) {
    return heightCheck->getGroundHeight();
}

void rs::calcGroundNormalExceptJustLandOrGravityDir(sead::Vector3<float>* vec, al::LiveActor const* actor, IUsePlayerCollision const* collider) {
    if(!rs::isJustLand(collider) && rs::isCollidedGround(collider)) {
        *vec = rs::getCollidedGroundNormal(collider);
    } else {
        *vec = -al::getGravity(actor);
    }
}

const al::CollisionParts* rs::getCollidedGroundCollisionParts(IUsePlayerCollision const* collider) {
    return collider->getPlayerCollider()->info1->mTriangle.mCollisionParts;
}

const al::CollisionParts* rs::getCollidedWallCollisionParts(IUsePlayerCollision const* collider) {
    return collider->getPlayerCollider()->info2->mTriangle.mCollisionParts;
}


const sead::Vector3f& rs::getCollidedCeilingNormal(IUsePlayerCollision const* collider) {
    return collider->getPlayerCollider()->info3->mTriangle.getFaceNormal();
}
