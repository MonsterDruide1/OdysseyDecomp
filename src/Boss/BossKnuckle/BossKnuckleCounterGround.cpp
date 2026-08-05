#include "Boss/BossKnuckle/BossKnuckleCounterGround.h"

#include <math/seadQuat.h>

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BossKnuckleCounterGround, WaitOnGround);
NERVE_IMPL(BossKnuckleCounterGround, Break);
NERVE_IMPL(BossKnuckleCounterGround, Fall);
NERVE_IMPL(BossKnuckleCounterGround, BeforeStart);
NERVE_IMPL(BossKnuckleCounterGround, Sink);
NERVES_MAKE_NOSTRUCT(BossKnuckleCounterGround, WaitOnGround, Break, Fall, BeforeStart, Sink);
}  // namespace

BossKnuckleCounterGround::BossKnuckleCounterGround(const char* name)
    : al::LiveActor(name), mBreakActor(nullptr), mUnusedActor(nullptr), mLifeUpItem(nullptr),
      mFallStartTrans(0.0f, 0.0f, 0.0f), mIsBreakOnGround(false), mIsBreakByIceConflict(false) {}

void BossKnuckleCounterGround::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "BossKnuckleCounterGround", nullptr);
    al::initNerve(this, &WaitOnGround, 0);

    mBreakActor = new al::LiveActor("氷壊れ");
    al::initChildActorWithArchiveNameNoPlacementInfo(mBreakActor, initInfo,
                                                     "BossKnuckleCounterGroundBreak", nullptr);

    bool hasLifeUp = false;
    al::tryGetArg(&hasLifeUp, initInfo, "HasLifeUp");
    if (hasLifeUp) {
        mLifeUpItem = new al::LiveActor("ライフアップアイテム");
        al::initChildActorWithArchiveNameNoPlacementInfo(mLifeUpItem, initInfo, "LifeUpItem",
                                                         nullptr);
        al::setTrans(mLifeUpItem, al::getTrans(this));
        mLifeUpItem->makeActorAlive();
        al::offCollide(mLifeUpItem);
        rs::tryInitItem(this, rs::ItemType::LifeUpItem, initInfo, false);
    }

    mBreakActor->makeActorDead();
    al::invalidateClipping(this);
    makeActorAlive();
}

void BossKnuckleCounterGround::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &Break))
        return;

    if (al::isSensorEnemyBody(other) && rs::sendMsgBossKnuckleCounter(other, self)) {
        mIsBreakByIceConflict = false;
        if (al::isAlive(this)) {
            mIsBreakOnGround = al::isNerve(this, &WaitOnGround);
            al::setNerve(this, &Break);
        }
    }

    if (al::isNerve(this, &Fall)) {
        if (al::sendMsgEnemyAttack(other, self)) {
            mIsBreakByIceConflict = false;
            al::startHitReaction(this, "氷衝突");
            if (al::isAlive(this)) {
                mIsBreakOnGround = al::isNerve(this, &WaitOnGround);
                al::setNerve(this, &Break);
            }
        }

        rs::sendMsgBossKnuckleIceFallToMummy(other, self);
        rs::sendMsgBossKnuckleIceConflict(other, self);
    }
}

void BossKnuckleCounterGround::doBreak() {
    if (al::isAlive(this)) {
        mIsBreakOnGround = al::isNerve(this, &WaitOnGround);
        al::setNerve(this, &Break);
    }
}

bool BossKnuckleCounterGround::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                          al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (al::isNerve(this, &Break))
        return false;

    if (al::isNerve(this, &Fall))
        return false;

    if (!rs::isMsgBossKnuckleIceConflict(message))
        return false;

    if (al::isNerve(this, &WaitOnGround))
        al::startHitReaction(this, "氷衝突（埋まり)");
    else
        al::startHitReaction(this, "氷衝突");

    mIsBreakByIceConflict = true;
    al::offCollide(this);
    if (al::isAlive(this)) {
        mIsBreakOnGround = al::isNerve(this, &WaitOnGround);
        al::setNerve(this, &Break);
    }

    return true;
}

void BossKnuckleCounterGround::doFall(const sead::Vector3f& trans) {
    al::offCollide(this);
    al::invalidateCollisionParts(this);
    al::showModelIfHide(this);
    al::invalidateClipping(this);

    mFallStartTrans.set(trans);
    sead::Vector3f startTrans = trans;
    startTrans.y += 1400.0f;
    al::resetPosition(this, startTrans);
    al::setUp(this, sead::Vector3f::ey);
    al::setVelocityY(this, -20.0f);
    al::validateShadow(this);
    appear();
    al::setNerve(this, &Fall);
}

void BossKnuckleCounterGround::doFallStartDemo() {
    appear();
    al::offCollide(this);
    al::invalidateCollisionParts(this);
    al::showModelIfHide(this);
    al::invalidateClipping(this);

    mFallStartTrans.set(al::getTrans(this));
    mFallStartTrans.y += 200.0f;
    sead::Vector3f startTrans = mFallStartTrans;
    startTrans.y += 3500.0f;
    al::resetPosition(this, startTrans);
    al::setUp(this, sead::Vector3f::ey);
    al::setVelocityY(this, -120.0f);

    if (mLifeUpItem)
        al::resetPosition(mLifeUpItem, startTrans + sead::Vector3f::ey * 105.0f);

    al::setNerve(this, &Fall);
}

void BossKnuckleCounterGround::setBeforeStart() {
    al::setNerve(this, &BeforeStart);
}

void BossKnuckleCounterGround::doWaitOnGround() {
    al::startAction(this, "Sink");
    al::setActionFrame(this, al::getActionFrameMax(this, "Sink"));
    al::offCollide(this);
    al::setUp(this, sead::Vector3f::ey);
    al::invalidateShadow(this);

    if (mLifeUpItem)
        al::resetPosition(mLifeUpItem, al::getTrans(this) + sead::Vector3f::ey * 105.0f);

    al::setNerve(this, &WaitOnGround);
}

bool BossKnuckleCounterGround::isBreak() const {
    return al::isNerve(this, &Break);
}

bool BossKnuckleCounterGround::isBeforeStart() const {
    return al::isNerve(this, &BeforeStart);
}

void BossKnuckleCounterGround::killAll() {
    mBreakActor->kill();
    if (mLifeUpItem)
        mLifeUpItem->kill();
    kill();
}

void BossKnuckleCounterGround::exeWaitOnGround() {
    if (al::isFirstStep(this))
        al::validateCollisionParts(this);
}

void BossKnuckleCounterGround::exeFall() {
    if (al::isFirstStep(this)) {
        al::onCollide(this);
        al::startAction(this, "Wait");
    }

    al::turnFront(this, 4.0f);

    if (mLifeUpItem)
        al::setTrans(mLifeUpItem, al::getTrans(this) + sead::Vector3f::ey * 105.0f);

    if (al::isOnGround(this, 0)) {
        f32 groundOffset = al::getTrans(this).y - al::getColliderRadius(this) - mFallStartTrans.y;
        if (al::isNearZeroOrLess(groundOffset)) {
            al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
            al::offCollide(this);
            al::setNerve(this, &Sink);
        }
    }
}

void BossKnuckleCounterGround::exeSink() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Sink");
        al::setVelocityZero(this);
        al::setVelocityY(this, -15.0f);
        al::startHitReaction(this, "埋まり始め");
    }

    al::turnFront(this, 3.0f);

    if (mLifeUpItem)
        al::setTrans(mLifeUpItem, al::getTrans(this) + sead::Vector3f::ey * 105.0f);

    if (al::isGreaterEqualStep(this, 26)) {
        al::invalidateShadow(this);
        al::setVelocityZero(this);
        al::setNerve(this, &WaitOnGround);
    }
}

void BossKnuckleCounterGround::exeBreak() {
    if (al::isFirstStep(this)) {
        if (!mIsBreakByIceConflict) {
            al::setTrans(mBreakActor, al::getTrans(this));
            al::copyPose(mBreakActor, this);
            mBreakActor->makeActorAlive();
            al::invalidateClipping(mBreakActor);
            if (mIsBreakOnGround)
                al::startAction(mBreakActor, "BreakGround");
            else
                al::startAction(mBreakActor, "BreakAir");
            al::hideModelIfShow(this);
            al::invalidateCollisionParts(this);
        }

        if (mLifeUpItem) {
            const sead::Vector3f& trans = al::getTrans(this);
            sead::Vector3f itemTrans =
                sead::Vector3f::ey * 105.0f + sead::Vector3f::ey * 105.0f + trans;
            const sead::Quatf& itemQuat = al::getQuat(mLifeUpItem);
            al::appearItem(this, itemTrans, itemQuat, nullptr);
            mLifeUpItem->kill();
            mLifeUpItem = nullptr;
        }
    }

    if (mIsBreakByIceConflict) {
        kill();
        return;
    }

    if (al::isActionEnd(mBreakActor) && al::isGreaterEqualStep(this, 1)) {
        mIsBreakOnGround = false;
        al::validateClipping(mBreakActor);
        mBreakActor->kill();
        kill();
    }
}

void BossKnuckleCounterGround::exeBeforeStart() {}
