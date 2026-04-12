#include "Enemy/DonkeyKong2D.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/Barrel2D.h"
#include "MapObj/BarrelStack2D.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(DonkeyKong2D, Wait);
NERVE_IMPL(DonkeyKong2D, Down);
NERVE_IMPL(DonkeyKong2D, Damage);
NERVE_IMPL(DonkeyKong2D, Throw);

NERVES_MAKE_STRUCT(DonkeyKong2D, Down, Damage);
NERVES_MAKE_NOSTRUCT(DonkeyKong2D, Wait, Throw);
}  // namespace

static const sead::Vector3f sThrowOffset = {0.0f, 0.0f, -320.0f};

DonkeyKong2D::DonkeyKong2D(const char* name) : al::LiveActor(name) {}

void DonkeyKong2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    rs::createAndSetFilter2DOnly(this);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    if (!rs::isIn2DArea(this)) {
        makeActorDead();
        return;
    }

    rs::snap2D(this, this, 500.0f);
    al::tryGetArg(&mWaitStep, info, "WaitStep");
    al::tryGetArg(&mMoveSpeed, info, "MoveSpeed");

    s32 barrelNum = 3;
    al::tryGetArg(&barrelNum, info, "BarrelNum");

    if (al::calcLinkChildNum(info, "BarrelStack") == 1) {
        mBarrelStack = new BarrelStack2D("積まれたタル2D");
        al::initLinksActor(mBarrelStack, info, "BarrelStack", 0);
    }

    al::DeriveActorGroup<Barrel2D>* barrelGroup =
        new al::DeriveActorGroup<Barrel2D>("樽グループ", barrelNum);
    mBarrelGroup = barrelGroup;
    for (s32 i = 0; i < barrelGroup->getMaxActorCount(); i++) {
        Barrel2D* barrel = new Barrel2D("樽2D");
        al::initCreateActorWithPlacementInfo(barrel, info);
        barrelGroup->registerActor(barrel);
    }

    mBarrelGroup->makeActorDeadAll();
    al::onCollide(this);
    makeActorAlive();
}

void DonkeyKong2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isNerve(this, &NrvDonkeyKong2D.Down) && al::isSensorEnemyAttack(self))
        rs::sendMsgEnemyAttack2D(other, self) || rs::sendMsgPush2D(other, self);
}

bool DonkeyKong2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (al::isNerve(this, &NrvDonkeyKong2D.Down))
        return false;

    if (al::isNerve(this, &NrvDonkeyKong2D.Damage) && al::isLessEqualStep(this, 10))
        return false;

    if (rs::isMsgBlockUpperPunch2D(message)) {
        mHitsTaken++;
        rs::requestHitReactionToAttacker(message, self, other);
        if (mHitsTaken >= 4) {
            al::startHitReaction(this, "ダウン");
            al::setNerve(this, &NrvDonkeyKong2D.Down);
        } else {
            al::startHitReaction(this, "ダメージ");
            al::setNerve(this, &NrvDonkeyKong2D.Damage);
        }

        return true;
    }

    return false;
}

void DonkeyKong2D::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::validateClipping(this);
    }

    if (al::isGreaterEqualStep(this, mWaitStep)) {
        mHeldBarrel = mBarrelGroup->tryFindDeadDeriveActor();
        if (mHeldBarrel)
            al::setNerve(this, &Throw);
    }
}

void DonkeyKong2D::exeThrow() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Throw");
        al::invalidateClipping(this);
    }

    if (mHeldBarrel && al::isGreaterEqualStep(this, 59)) {
        sead::Vector3f trans;
        al::calcTransLocalOffset(&trans, this, sThrowOffset);
        sead::Quatf quat = al::getQuat(this);
        al::rotateQuatYDirDegree(&quat, quat, 180.0f);
        mHeldBarrel->appearByGenerator(trans, quat, mMoveSpeed);
        mHeldBarrel->startMove();
        mHeldBarrel = nullptr;
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void DonkeyKong2D::exeDamage() {
    if (al::isFirstStep(this))
        al::startAction(this, "Damage");

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void DonkeyKong2D::exeDown() {
    sead::Vector3f gravity;
    rs::calcDimensionGravity(&gravity, this, al::getGravity(this));

    if (al::isStep(this, 1)) {
        al::startAction(this, "Down");
        al::tryOnSwitchDeadOn(this);
        al::offCollide(this);
        al::invalidateClipping(this);

        al::setVelocityToDirection(this, -gravity, 36.0f);
        if (mBarrelStack)
            mBarrelStack->doBreak();
    }

    if (al::isGreaterStep(this, 1)) {
        al::addVelocityToDirection(this, gravity, 1.3f);
        al::scaleVelocity(this, 0.98f);
    }

    if (al::isStep(this, 20))
        al::startHitReaction(this, "落下開始");

    if (al::isGreaterEqualStep(this, 270))
        kill();
}
