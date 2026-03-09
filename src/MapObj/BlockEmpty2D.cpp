#include "MapObj/BlockEmpty2D.h"

#include <prim/seadSafeString.h>

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/SensorMsgFunction.h"

BlockEmpty2D::BlockEmpty2D(const char* name) : al::LiveActor(name) {}

void BlockEmpty2D::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BlockEmpty2D", nullptr);
    mMtxConnector = al::tryCreateMtxConnector(this, info);
    if (mMtxConnector)
        al::tryGetArg(&mIsConnectToCollisionBack, info, "IsConnectToCollisionBack");
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (rs::isIn2DArea(this)) {
        rs::snap2DParallelizeFront(this, this, 500.0f);
        makeActorAlive();
    } else {
        al::PlacementId placementId;
        al::tryGetPlacementId(&placementId, info);
        sead::FixedSafeString<128> str;
        placementId.makeString(&str);
        kill();
    }
}

void BlockEmpty2D::initAfterPlacement() {
    if (mMtxConnector) {
        if (mIsConnectToCollisionBack) {
            sead::Vector3f frontDir = sead::Vector3f::zero;
            al::calcFrontDir(&frontDir, this);
            al::MtxConnector* connector = mMtxConnector;
            const sead::Vector3f& trans = al::getTrans(this);
            sead::Vector3f pos = {frontDir.x * 50.0f + trans.x, frontDir.y * 50.0f + trans.y,
                                  frontDir.z * 50.0f + trans.z};
            sead::Vector3f dir = {frontDir.x * -400.0f, frontDir.y * -400.0f, frontDir.z * -400.0f};
            al::attachMtxConnectorToCollision(connector, this, pos, dir);
        } else {
            al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
        }
    }
}

void BlockEmpty2D::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void BlockEmpty2D::appear() {
    al::LiveActor::appear();
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

bool BlockEmpty2D::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(msg))
        return true;
    if (al::isMsgPlayerDisregard(msg))
        return true;
    if (rs::isMsgPlayerUpperPunch2D(msg))
        al::startHitReaction(this, "アッパーパンチ");
    return false;
}

ActorDimensionKeeper* BlockEmpty2D::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}
