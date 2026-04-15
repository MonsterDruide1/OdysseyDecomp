#include "MapObj/ShopMark.h"

#include <math/seadVector.h>

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Matrix/MatrixUtil.h"

#include "Layout/BalloonIcon.h"
#include "Util/DemoUtil.h"

ShopMark::ShopMark(const char* name) : al::LiveActor(name) {}

void ShopMark::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ShopMark", nullptr);
    mBalloonIcon =
        rs::createShopBalloon(al::getLayoutInitInfo(info), &mBalloonMtx, sead::Vector3f::zero);
    mMtxConnector = al::tryCreateMtxConnector(this, info);
    rs::addDemoActor(this, false);
    makeActorAlive();
}

void ShopMark::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);

    mBalloonIcon->startUpdateDraw();
}

void ShopMark::startClipped() {
    al::LiveActor::startClipped();
    mBalloonIcon->hideAndStopUpdate();
}

void ShopMark::endClipped() {
    al::LiveActor::endClipped();
    mBalloonIcon->startUpdateDraw();
}

void ShopMark::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);

    al::makeMtxQuatPos(&mBalloonMtx, al::getQuat(this), al::getTrans(this));
}

bool ShopMark::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;
    return false;
}
