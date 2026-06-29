#include "Item/LifeUpItem2D.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Item/LifeUpItemStateAutoGet.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(LifeUpItem2D, Wait);
NERVE_IMPL(LifeUpItem2D, AutoGetDemo);
NERVE_IMPL(LifeUpItem2D, PopUp);
NERVE_IMPL(LifeUpItem2D, Get);

NERVES_MAKE_NOSTRUCT(LifeUpItem2D, Get);
NERVES_MAKE_STRUCT(LifeUpItem2D, Wait, AutoGetDemo, PopUp);

void appearIn2DArea(LifeUpItem2D* actor, ActorDimensionKeeper* dimensionKeeper);
}  // namespace

LifeUpItem2D::LifeUpItem2D(const char* name) : al::LiveActor(name) {}

void LifeUpItem2D::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "LifeUpItem2D", nullptr);
    al::initNerve(this, &NrvLifeUpItem2D.Wait, 1);
    al::offCollide(this);
    rs::createAndSetFilter2DOnly(this);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (al::isPlaced(info))
        rs::snap2DParallelizeFront(this, this, 500.0f);

    LifeUpItemStateAutoGet* stateAutoGet = new LifeUpItemStateAutoGet(this, true);
    mStateAutoGet = stateAutoGet;
    al::initNerveState(this, stateAutoGet, &NrvLifeUpItem2D.AutoGetDemo, "自動取得");
    al::startAction(this, "Wait");
    makeActorAlive();
}

void LifeUpItem2D::appearPopUp() {
    al::setNerve(this, &NrvLifeUpItem2D.PopUp);
    appearIn2DArea(this, mDimensionKeeper);
    al::startHitReaction(this, "飛出し出現");
}

namespace {
void appearIn2DArea(LifeUpItem2D* actor, ActorDimensionKeeper* dimensionKeeper) {
    rs::updateDimensionKeeper(dimensionKeeper);
    if (!rs::isIn2DArea(actor))
        return;

    al::onCollide(actor);
    rs::snap2DParallelizeFront(actor, actor, 500.0f);
    al::setVelocity(actor, al::getGravity(actor) * -11.0f);
    al::showModelIfHide(actor);
    al::validateHitSensors(actor);
    al::invalidateClipping(actor);
    al::startAction(actor, "AppearPopUp");
    actor->appear();
}
}  // namespace

void LifeUpItem2D::appearAmiiboTouch() {
    al::setNerve(this, &NrvLifeUpItem2D.AutoGetDemo);
    appearIn2DArea(this, mDimensionKeeper);
    al::startHitReaction(this, "飛出し出現");
}

void LifeUpItem2D::get() {
    if (al::isNerve(this, &Get))
        return;

    al::setVelocityZero(this);
    al::startHitReaction(this, "取得");
    GameDataHolderAccessor accessor(this);
    if (!GameDataFunction::isPlayerHitPointMax(accessor)) {
        GameDataFunction::recoveryPlayer(this);
        kill();
        return;
    }

    if (mIsAmiiboTouch) {
        kill();
        return;
    }

    if (!mHitSensor)
        mHitSensor = al::getHitSensor(this, "Body");

    al::hideModel(this);
    al::invalidateHitSensors(this);
    al::invalidateClipping(this);
    al::setNerve(this, &Get);
}

void LifeUpItem2D::exePopUp() {
    al::addVelocityToGravity(this, 0.4f);
    al::limitVelocitySeparateHV(this, al::getGravity(this), 10.0f, 20.0f);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        kill();
        return;
    }

    rs::snap2DParallelizeFront(this, this, 500.0f);
    if (al::isOnGround(this, 0)) {
        al::offCollide(this);
        al::setVelocityZero(this);
        al::validateClipping(this);
        al::setNerve(this, &NrvLifeUpItem2D.Wait);
    }
}

void LifeUpItem2D::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void LifeUpItem2D::exeGet() {
    if (al::isFirstStep(this))
        mCoinAppearCount = 0;

    if (rs::tryAppearMultiCoinFromObj(this, mHitSensor, al::getNerveStep(this), 150.0f))
        mCoinAppearCount++;

    if (mCoinAppearCount >= 5)
        kill();
}

void LifeUpItem2D::exeAutoGetDemo() {
    if (al::updateNerveState(this))
        get();
}

bool LifeUpItem2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (!rs::isMsgItemGet2D(message) || ((al::isNerve(this, &NrvLifeUpItem2D.PopUp) ||
                                          al::isNerve(this, &NrvLifeUpItem2D.AutoGetDemo)) &&
                                         al::isLessEqualStep(this, 40))) {
        return false;
    }

    mHitSensor = other;
    get();
    return true;
}
