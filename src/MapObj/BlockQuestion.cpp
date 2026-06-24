#include "MapObj/BlockQuestion.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Item/RandomItemSelector.h"
#include "MapObj/BlockEmpty.h"
#include "MapObj/BlockStateSingleItem.h"
#include "MapObj/BlockStateTenCoin.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"

namespace {
NERVE_IMPL(BlockQuestion, Dead);
NERVE_IMPL(BlockQuestion, TenCoin);
NERVE_IMPL(BlockQuestion, SingleItem);

NERVES_MAKE_STRUCT(BlockQuestion, TenCoin, SingleItem);
NERVES_MAKE_NOSTRUCT(BlockQuestion, Dead);
}  // namespace

BlockQuestion::BlockQuestion(const char* name) : al::LiveActor(name) {}

void BlockQuestion::init(const al::ActorInitInfo& initInfo) {
    using BlockQuestionFunctor = al::FunctorV0M<BlockQuestion*, void (BlockQuestion::*)()>;

    bool isPlayWaitSe = false;
    const char* suffix = nullptr;
    if (al::tryGetArg(&isPlayWaitSe, initInfo, "IsPlayWaitSe") && isPlayWaitSe)
        suffix = "PlayWaitSe";
    al::initMapPartsActor(this, initInfo, suffix);

    mItemType = rs::getItemType(initInfo);
    if (mItemType > rs::ItemType::ValueType::None)
        rs::initItemByPlacementInfo(this, initInfo, false);

    al::tryAddDisplayOffset(this, initInfo);
    mShadowLength = rs::setShadowDropLength(this, initInfo, "本体");
    al::expandClippingRadiusByShadowLength(this, &_134, mShadowLength);

    if (mItemType == rs::ItemType::ValueType::Coin10) {
        mBlockStateTenCoin = new BlockStateTenCoin(this, false);
        al::initNerve(this, &NrvBlockQuestion.TenCoin, 1);
        al::initNerveState(this, mBlockStateTenCoin, &NrvBlockQuestion.TenCoin, "10コイン");
    } else {
        mBlockStateSingleItem = new BlockStateSingleItem(this, mItemType, false);
        al::initNerve(this, &NrvBlockQuestion.SingleItem, 1);
        al::initNerveState(this, mBlockStateSingleItem, &NrvBlockQuestion.SingleItem, "アイテム");
        if (mItemType == rs::ItemType::ValueType::Random)
            rs::createRandomItemSelector(this);
    }

    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    if (mMtxConnector)
        al::tryGetArg(&mIsConnectToCollisionBack, initInfo, "IsConnectToCollisionBack");

    if (al::isEqualString(getName(), "都市ワールドホーム信号機004"))
        mBlockEmpty = new BlockEmpty("空ブロック", "CityWorldHomeSignal005");
    else
        mBlockEmpty = new BlockEmpty("空ブロック", "BlockEmpty");
    al::initCreateActorWithPlacementInfo(mBlockEmpty, initInfo);
    mBlockEmpty->setShadowDropLength(mShadowLength);
    mBlockEmpty->kill();

    if (al::listenStageSwitchOnAppear(this,
                                      BlockQuestionFunctor(this, &BlockQuestion::listenApeear))) {
        makeActorDead();
    } else {
        makeActorAlive();
    }

    al::listenStageSwitchOnKill(this, BlockQuestionFunctor(this, &BlockQuestion::listenKill));
}

void BlockQuestion::listenApeear() {
    appear();
}

void BlockQuestion::listenKill() {
    if (!al::isDead(mBlockEmpty))
        mBlockEmpty->kill();
    else
        kill();
}

void BlockQuestion::initAfterPlacement() {
    if (!mMtxConnector)
        return;

    if (!mIsConnectToCollisionBack) {
        al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
        return;
    }

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, this);
    al::attachMtxConnectorToCollision(mMtxConnector, this, al::getTrans(this) + 50.0f * front,
                                      -400.0f * front);
}

void BlockQuestion::appear() {
    al::LiveActor::appear();
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void BlockQuestion::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

bool BlockQuestion::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgHammerBrosHammerEnemyAttack(message))
        return false;

    if (rs::isMsgBlockReaction3D(message)) {
        if (al::isSensorMapObj(self) && al::isMsgPlayerSpinAttack(message))
            return false;

        const sead::Vector3f& otherPos = al::getSensorPos(other);
        const sead::Vector3f& selfPos = al::getSensorPos(self);
        if (al::isNerve(this, &NrvBlockQuestion.SingleItem) &&
            mBlockStateSingleItem->isEnableAppearItem()) {
            if (mItemType != rs::ItemType::ValueType::None)
                mBlockStateSingleItem->setItemOffsetY(150.0f);

            if (rs::isMsgCapAttack(message)) {
                if ((otherPos - selfPos).length() > 150.0f)
                    return false;
                mBlockStateSingleItem->receiveMsg(message, other, self);
                return false;
            }

            if (mBlockStateSingleItem->receiveMsg(message, other, self)) {
                if (!rs::isMsgUpperPunchAll(message))
                    rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
        } else if (al::isNerve(this, &NrvBlockQuestion.TenCoin)) {
            mBlockStateTenCoin->setItemOffsetY(150.0f);
            if (rs::isMsgCapAttack(message) && (otherPos - selfPos).length() > 150.0f)
                return false;

            if (mBlockStateTenCoin->receiveMsg(message, other, self)) {
                if (!rs::isMsgUpperPunchAll(message) &&
                    !rs::isMsgCapAttackStayRollingCollide(message))
                    rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
        }
    }

    if (rs::isMsgTRexAttackCollideAll(message)) {
        al::hideModelIfShow(this);
        al::appearBreakModelRandomRotateY(al::getSubActor(this, "壊れモデル"));
        al::invalidateHitSensors(this);
        al::invalidateCollisionParts(this);
        al::invalidateClipping(this);
        if (al::isNerve(this, &NrvBlockQuestion.SingleItem)) {
            mBlockStateSingleItem->autoGet(self);
            kill();
            return true;
        }

        if (al::isNerve(this, &NrvBlockQuestion.TenCoin))
            mBlockStateTenCoin->autoGet(self);
    }
    return false;
}

void BlockQuestion::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "BlockUpperPunch") &&
        ((al::isNerve(this, &NrvBlockQuestion.SingleItem) && mBlockStateSingleItem->isReaction()) ||
         (al::isNerve(this, &NrvBlockQuestion.TenCoin) && mBlockStateTenCoin->isReaction()))) {
        al::sendMsgBlockUpperPunch(other, self, nullptr);
    }
}

void BlockQuestion::exeSingleItem() {
    if (al::updateNerveState(this)) {
        mBlockEmpty->appear();
        al::setNerve(this, &Dead);
    }
}

void BlockQuestion::exeTenCoin() {
    if (al::updateNerveState(this)) {
        if (!al::isHideModel(this))
            mBlockEmpty->appear();
        al::setNerve(this, &Dead);
    }
}

void BlockQuestion::exeDead() {
    if (al::isFirstStep(this))
        kill();
}
