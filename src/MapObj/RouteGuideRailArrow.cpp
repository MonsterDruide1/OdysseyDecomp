#include "MapObj/RouteGuideRailArrow.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/ActorDimensionUtil.h"

RouteGuideRailArrow::RouteGuideRailArrow(const char* name) : al::LiveActor(name) {
    mCollisionPartsConnector = new al::CollisionPartsConnector();
}

void RouteGuideRailArrow::makeActorAlive() {
    if (isValidateGuide()) {
        al::LiveActor::makeActorAlive();
        al::connectPoseQT(this, mCollisionPartsConnector);
    }
}

void RouteGuideRailArrow::makeActorDead() {
    al::LiveActor::makeActorDead();
}

void RouteGuideRailArrow::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);
    al::tryGetArg(&mDisplayOffsetY, info, "DisplayOffsetY");
    al::initJointControllerKeeper(this, 1);
    al::initJointLocalTransControllerY(this, &mDisplayOffsetY, "KidsGuide3DArrow");
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    if (rs::isIn2DArea(this))
        rs::snap2DUp(this, this, 500.0f);

    if (isValidateGuide())
        al::LiveActor::makeActorAlive();
    else
        al::LiveActor::makeActorDead();
}

void RouteGuideRailArrow::control() {
    if (mCollisionPartsConnector->isConnectInvalidCollision() && al::isAlive(this))
        makeActorDead();

    if (mCollisionPartsConnector->isMoved())
        al::connectPoseQT(this, mCollisionPartsConnector);
}

const sead::Vector3f& RouteGuideRailArrow::getGuideTrans() const {
    return al::getTrans(this);
}

void RouteGuideRailArrow::setGuideAlpha(f32 alpha) {
    if (mCollisionPartsConnector->isConnectInvalidCollision())
        return;

    if (0.0f >= alpha) {
        if (al::isAlive(this))
            makeActorDead();
        return;
    }

    if (al::isDead(this))
        makeActorAlive();
    al::setModelAlphaMask(this, alpha);
}

void RouteGuideRailArrow::invalidateGuide() {
    if (al::isAlive(this))
        makeActorDead();

    RouteGuideArrowBase::invalidateGuide();
}

void RouteGuideRailArrow::attach(const al::CollisionParts* collisionParts) {
    al::attachCollisionPartsConnector(mCollisionPartsConnector, collisionParts);
}

void RouteGuideRailArrow::setBaseQuatTrans() {
    const sead::Quatf& quat = al::getQuat(this);
    const sead::Vector3f& trans = al::getTrans(this);
    al::setConnectorBaseQuatTrans(quat, trans, mCollisionPartsConnector);
}
