#include "MapObj/LinkTargetPoint.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Collision/PartsMtxConnector.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/TouchTargetInfo.h"
#include "Util/SensorMsgFunction.h"

LinkTargetPoint::LinkTargetPoint() : al::LiveActor("リンク対象位置") {}

void LinkTargetPoint::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorMapObjMovement(this, info);
    al::initActorPoseTQSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::calcUpDir(&mNormal, this);

    bool isConnect = false;
    al::tryGetArg(&isConnect, info, "IsConnectToCollision");
    if (isConnect)
        mMtxConnector = al::createMtxConnector(this);

    makeActorAlive();
}

void LinkTargetPoint::initAfterPlacement() {
    if (!mMtxConnector)
        return;

    al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 100.0f);
    if (alCollisionUtil::getStrikeArrowInfoNum(this) >= 1)
        mNormal = (*alCollisionUtil::getStrikeArrowInfo(this, 0))->triangle.getFaceNormal();
}

void LinkTargetPoint::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void LinkTargetPoint::calcTargetCenterPos(sead::Vector3f* pos) const {
    al::MtxConnector* connector = mMtxConnector;
    if (connector) {
        const sead::Vector3f& baseTrans = al::getConnectBaseTrans(connector);
        connector->multTrans(pos, baseTrans);
    } else {
        *pos = al::getTrans(this);
    }
}

bool LinkTargetPoint::receiveMsgInitTouchTargetInfo(const al::SensorMsg* msg) {
    TouchTargetInfo* info;
    if (!rs::tryGetTouchTargetInfo(&info, msg))
        return false;

    if (mMtxConnector) {
        info->setInfoByConnector(mMtxConnector, al::getConnectBaseTrans(mMtxConnector), mNormal,
                                 true);
    } else {
        info->setInfoByPosAndNrm(al::getTrans(this), mNormal);
    }

    return true;
}
