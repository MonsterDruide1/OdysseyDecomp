#include "Item/CoinCollectEmpty2D.h"

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Model/ModelShapeUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CoinCollectEmpty2D, Wait);
NERVE_IMPL(CoinCollectEmpty2D, Got);

NERVES_MAKE_STRUCT(CoinCollectEmpty2D, Wait, Got);
}  // namespace

CoinCollectEmpty2D::CoinCollectEmpty2D(const char* name, const char* archiveName)
    : al::LiveActor(name), IUseDimension(), mArchiveName(archiveName) {}

void CoinCollectEmpty2D::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::initActorWithArchiveName(this, initInfo, mArchiveName, nullptr);
    al::tryAddDisplayOffset(this, initInfo);
    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    rs::snap2DParallelizeFront(this, this, 500.0f);
    al::initNerve(this, &NrvCoinCollectEmpty2D.Wait, 0);
    al::startAction(this, "Wait");
    makeActorAlive();
}

void CoinCollectEmpty2D::initAfterPlacement() {
    if (mMtxConnector != nullptr)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);

    sead::Matrix44f matrix = sead::Matrix44f::ident;
    sead::Vector3f frontDir = sead::Vector3f::zero;
    sead::Vector3f upDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, this);
    al::calcUpDir(&upDir, this);
    const sead::Vector2f aDir(100.0f, 100.0f);
    al::makeMtxProj(&matrix, aDir, frontDir, upDir);
    const sead::Vector3f& objTrans = al::getTrans(this);
    matrix.setCol(3, sead::Vector4f(objTrans.x, objTrans.y, objTrans.z, 1.0f));
    al::setModelProjMtx0(getModelKeeper(), matrix);
}

bool CoinCollectEmpty2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (rs::isMsgItemGet2D(message) && al::isNerve(this, &NrvCoinCollectEmpty2D.Wait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollectEmpty2D.Got);
        return true;
    }
    if (al::isMsgPlayerDisregard(message))
        return true;
    return false;
}

void CoinCollectEmpty2D::endClipped() {
    rs::syncCoin2DAnimFrame(this, "Wait");
    al::LiveActor::endClipped();
}

ActorDimensionKeeper* CoinCollectEmpty2D::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}

void CoinCollectEmpty2D::exeWait() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);
}

void CoinCollectEmpty2D::exeGot() {
    if (al::isFirstStep(this))
        al::startAction(this, "Got");

    if (al::isActionEnd(this)) {
        GameDataFunction::addCoin(this, 1);
        GameDataFunction::addCoin(this, 1);
        kill();
    }
}
