#include "MapObj/BlockQuestion2D.h"

#include <math/seadVector.h>

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Stage/StageRhythm.h"

#include "MapObj/BlockEmpty2D.h"
#include "MapObj/BlockStateSingleItem.h"
#include "MapObj/BlockStateTenCoin.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BlockQuestion2D, SingleItem);
NERVE_IMPL(BlockQuestion2D, TenCoin);

NERVES_MAKE_STRUCT(BlockQuestion2D, TenCoin, SingleItem);
}  // namespace

BlockQuestion2D::BlockQuestion2D(const char* name) : al::LiveActor(name) {}

void BlockQuestion2D::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BlockQuestion2D", nullptr);
    mItemType = rs::getItemType(info);

    if (mItemType < 0 ||
        (rs::initItem2DByPlacementInfo(this, info), mItemType != rs::ItemType::Coin10)) {
        mSingleItemState = new BlockStateSingleItem(this, mItemType, true);
        al::initNerve(this, &NrvBlockQuestion2D.SingleItem, 1);
        al::initNerveState(this, mSingleItemState, &NrvBlockQuestion2D.SingleItem, "アイテム");
    } else {
        mTenCoinState = new BlockStateTenCoin(this, true);
        al::initNerve(this, &NrvBlockQuestion2D.TenCoin, 1);
        al::initNerveState(this, mTenCoinState, &NrvBlockQuestion2D.TenCoin, "10コイン");
    }

    al::startAction(this, "Wait");
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    if (rs::isIn2DArea(this)) {
        mMtxConnector = al::tryCreateMtxConnector(this, info);
        if (mMtxConnector)
            al::tryGetArg(&mIsConnectToCollisionBack, info, "IsConnectToCollisionBack");

        rs::snap2DParallelizeFront(this, this, 500.0f);
        mEmptyBlock2D = new BlockEmpty2D("空ブロック2D");
        al::initCreateActorWithPlacementInfo(mEmptyBlock2D, info);
        mEmptyBlock2D->makeActorDead();
        makeActorAlive();
        return;
    }

    al::PlacementId placementId;
    al::tryGetPlacementId(&placementId, info);
    al::StringTmp<128> str;
    placementId.makeString(&str);
    kill();
}

void BlockQuestion2D::initAfterPlacement() {
    if (mMtxConnector) {
        if (mIsConnectToCollisionBack) {
            sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
            al::calcFrontDir(&frontDir, this);
            al::attachMtxConnectorToCollision(
                mMtxConnector, this, al::getTrans(this) + 50.0f * frontDir, -400.0f * frontDir);
        } else {
            al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
        }
    }
}

void BlockQuestion2D::appear() {
    al::LiveActor::appear();
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void BlockQuestion2D::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void BlockQuestion2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvBlockQuestion2D.SingleItem) && mSingleItemState->isReaction()) {
        rs::sendMsgBlockUpperPunch2D(other, self);
        return;
    }

    if (al::isNerve(this, &NrvBlockQuestion2D.TenCoin) && mTenCoinState->isReaction())
        rs::sendMsgBlockUpperPunch2D(other, self);
}

bool BlockQuestion2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (!rs::isMsgBlockReaction2D(message) || al::isSensorName(self, "UpperPunch"))
        return false;

    if (al::isNerve(this, &NrvBlockQuestion2D.SingleItem)) {
        mSingleItemState->setItemOffsetY(200.0f);
        return mSingleItemState->receiveMsg(message, other, self);
    }

    if (!al::isNerve(this, &NrvBlockQuestion2D.TenCoin))
        return false;

    mTenCoinState->mItemOffsetY = 200.0f;
    return mTenCoinState->receiveMsg(message, other, self);
}

void BlockQuestion2D::endClipped() {
    s32 syncCounter = al::getSceneObj<al::StageSyncCounter>(this)->getCounter();
    s32 frameMax = al::getVisAnimFrameMax(this, "Wait");
    al::setVisAnimFrameForAction(this, syncCounter % frameMax);
    al::LiveActor::endClipped();
}

void BlockQuestion2D::exeSingleItem() {
    if (al::updateNerveState(this)) {
        mEmptyBlock2D->appear();
        BlockEmpty2D* emptyBlock2D = mEmptyBlock2D;
        emptyBlock2D->setMtxConnector(mMtxConnector);
        kill();
    }
}

void BlockQuestion2D::exeTenCoin() {
    if (al::updateNerveState(this)) {
        mEmptyBlock2D->appear();
        kill();
    }
}

ActorDimensionKeeper* BlockQuestion2D::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}
