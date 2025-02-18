#include "Enemy/Popn.h"

#include <math/seadVector.h>

#include "Library/Collision/KCollisionServer.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Popn, Appear);
NERVE_IMPL(Popn, BlowDown);
NERVE_IMPL(Popn, Move);
NERVE_IMPL(Popn, Wait);
NERVE_IMPL(Popn, WaitRove);
NERVE_IMPL(Popn, MoveStart);
NERVE_IMPL(Popn, MoveEnd);
NERVE_IMPL(Popn, Turn);

NERVES_MAKE_NOSTRUCT(Popn, MoveEnd);
NERVES_MAKE_STRUCT(Popn, Appear, BlowDown, Move, Wait, Turn, WaitRove, MoveStart);
}  // namespace

const al::EnemyStateBlowDownParam sBlowDownParam("BlowDown", 15.0f, 30.0f, 1.0f, 0.997f, 20, 1);

Popn::Popn(const char* actorName) : al::LiveActor(actorName) {}

void Popn::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, nullptr);
    al::initNerve(this, &NrvPopn.Appear, 1);
    mStateBlowDown = new al::EnemyStateBlowDown(this, &sBlowDownParam, "吹き飛び状態");
    al::initNerveState(this, mStateBlowDown, &NrvPopn.BlowDown, "吹っ飛びやられ");

    al::setColliderFilterCollisionParts(this,
                                        new al::CollisionPartsFilterSpecialPurpose("MoveLimit"));
    al::setColliderReactMovePower(this, 1);

    al::initJointControllerKeeper(this, 1);
    al::initJointGlobalQuatController(this, &mQuat, "AllRoot");

    makeActorAlive();
}

void Popn::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvPopn.BlowDown))
        return;

    if (al::isNerve(this, &NrvPopn.Appear) && al::isLessStep(this, 80))
        return;

    if (al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other) &&
        !al::isNerve(this, &NrvPopn.Move))
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);

    if (al::isSensorEnemyAttack(self))
        rs::sendMsgPushToPlayer(other, self) || al::sendMsgEnemyAttack(other, self);
}

bool Popn::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgNpcScareByEnemy(message))
        return true;
    if (rs::isMsgKillByShineGet(message) || rs::isMsgKillByHomeDemo(message)) {
        al::tryKillEmitterAndParticleAll(this);
        kill();
        return true;
    }

    // TODO: Fix the hacky match here
    bool isDisregard = al::isMsgPlayerDisregard(message);
    auto* nrv = &NrvPopn.BlowDown;
    __asm__("" ::[nrv] "r"(nrv));
    if (isDisregard)
        return al::isNerve(this, nrv);

    __asm__("");
    if (al::isNerve(this, nrv))
        return false;

    if (al::isSensorEnemyAttack(self))
        return false;
    if (al::isNerve(this, &NrvPopn.Appear) && al::isLessStep(this, 15))
        return false;
    if (al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 2.0f))
        return true;

    if (rs::isMsgCapAttack(message) || rs::isMsgBlowDown(message) ||
        rs::isMsgSeedAttackHold(message) || rs::isMsgWanwanEnemyAttack(message) ||
        rs::isMsgTankExplosion(message) || rs::isMsgTankBullet(message)) {
        al::tryDeleteEffect(this, "PopnAppear");
        rs::requestHitReactionToAttacker(message, self, other);

        if (mIsGenerateItem)
            rs::setAppearItemFactorAndOffsetByMsg(this, message, other);

        mStateBlowDown->start(other, self);
        al::invalidateClipping(this);
        al::setNerve(this, &NrvPopn.BlowDown);

        if (rs::isMsgTankBullet(message) || rs::isMsgSeedAttackHold(message) ||
            rs::isMsgTankExplosion(message))
            return true;

        return al::isMsgKickStoneAttackHold(message);
    }

    return false;
}

void Popn::control() {
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);

    sead::Vector3f groundNormal = sead::Vector3f::ey;
    if (al::isOnGround(this, 0) && mIsFollowGroundNormal)
        groundNormal.set(al::getCollidedGroundNormal(this));

    sead::Vector3f up;
    al::calcQuatUp(&up, mQuat);
    al::lerpVec(&up, up, groundNormal, 0.18f);

    sead::Quatf upFront;
    al::makeQuatUpFront(&upFront, up, frontDir);
    mQuat.set(upFront);

    if (al::isInDeathArea(this) || al::isCollidedFloorCode(this, "DamageFire") ||
        al::isCollidedFloorCode(this, "Needle") || al::isCollidedFloorCode(this, "Poison") ||
        al::isInWater(this)) {
        al::startHitReaction(this, "死亡");
        kill();
    }
}

void Popn::appearByGenerater(const sead::Vector3f& pos, s32 color, bool isAppear,
                             bool isGenerateItem) {
    mIsGenerateItem = isGenerateItem;

    al::resetPosition(this, pos);
    al::setScale(this, sead::Vector3f::ones);

    al::LiveActor* nearestPlayerActor = al::tryFindNearestPlayerActor(this);
    if (nearestPlayerActor)
        al::faceToTarget(this, nearestPlayerActor);

    if (isAppear) {
        al::startAction(this, "Appear");
        al::setNerve(this, &NrvPopn.Appear);
    } else {
        al::startAction(this, "Wait");
        al::setNerve(this, &NrvPopn.Wait);
    }

    if (!mIsAngry) {
        al::startMtpAnimAndSetFrameAndStop(this, "Color", color);
        al::startMclAnimAndSetFrameAndStop(this, "Color", color);
    }

    appear();

    if (mIsAngry) {
        al::emitEffect(this, "RedEyes", nullptr);
        mAwakeDistance = 4500.0f;
    }
}

inline bool shouldAwake(f32 awakeDistance, al::LiveActor* popn) {
    al::LiveActor* nearestPlayerActor = al::tryFindNearestPlayerActor(popn);
    if (!nearestPlayerActor)
        return false;
    return al::calcDistance(nearestPlayerActor, al::getTrans(popn)) < awakeDistance;
}

inline void applyInertia(al::LiveActor* popn) {
    if (al::isOnGround(popn, 0)) {
        al::scaleVelocity(popn, 0.8f);
        al::addVelocityToGravityFittedGround(popn, 0.55f, 0);
    } else {
        al::scaleVelocityHV(popn, 0.997f, 0.99f);
        al::addVelocityToGravity(popn, 0.55f);
    }
}

void Popn::exeAppear() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Appear");

    applyInertia(this);

    if (al::isActionEnd(this)) {
        if (shouldAwake(mAwakeDistance, this))
            al::setNerve(this, &NrvPopn.Turn);
        else
            al::setNerve(this, &NrvPopn.Wait);
    }
}

void Popn::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    applyInertia(this);

    if (shouldAwake(mAwakeDistance, this) && al::isOnGround(this, 0)) {
        al::setNerve(this, &NrvPopn.Turn);
        return;
    }

    s32 nerveStep = al::getNerveStep(this);
    s32 actionFrameMax = al::getActionFrameMax(this, "Wait");
    if (nerveStep % actionFrameMax == 0) {
        if (al::getRandom(1.0f) < 0.3f) {
            al::setNerve(this, &NrvPopn.WaitRove);
            return;
        }
    }
}

void Popn::exeWaitRove() {
    if (al::isFirstStep(this))
        al::startAction(this, "WaitRove");

    applyInertia(this);

    if (shouldAwake(mAwakeDistance, this)) {
        al::setNerve(this, &NrvPopn.Turn);
        return;
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPopn.Wait);
}

void Popn::exeTurn() {
    if (al::isFirstStep(this))
        al::startAction(this, "Turn");

    applyInertia(this);

    if (!shouldAwake(mAwakeDistance + 500.0f, this)) {
        al::setNerve(this, &NrvPopn.Wait);
        return;
    }

    f32 turnSpeed = mIsAngry ? 7.0f : 6.0f;
    s32 nerveStep = al::getNerveStep(this);
    s32 maxNerveStep = mIsAngry ? 21 : 35;
    if (nerveStep >= 20)
        turnSpeed *= (f32)(maxNerveStep - nerveStep) / (f32)(maxNerveStep - 20);

    const sead::Vector3f& nearestPlayerPos = al::findNearestPlayerPos(this);
    const sead::Vector3f& trans = al::getTrans(this);
    sead::Vector3f diff;

    diff.x = nearestPlayerPos.x - trans.x;
    diff.y = nearestPlayerPos.y - trans.y;
    diff.z = nearestPlayerPos.z - trans.z;

    if (!al::tryNormalizeOrZero(&diff))
        al::calcFrontDir(&diff, this);

    if (rs::isPlayerHackTRex(this)) {
        diff.x = -diff.x;
        diff.y = -diff.y;
        diff.z = -diff.z;
    }

    al::turnToDirection(this, diff, turnSpeed);

    if (nerveStep >= maxNerveStep)
        return al::setNerve(this, &NrvPopn.MoveStart);
}

void Popn::exeMoveStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "MoveStart");

    applyInertia(this);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPopn.Move);
}

void Popn::exeMove() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Move");
        al::setVelocityZero(this);

        al::addVelocityToFront(this, mIsAngry ? 17.0f : 8.1f);
        al::addVelocityY(this, mIsAngry ? 15.0f : 12.2f);

        sead::Vector3f inertia;
        sead::Vector3f velocity = al::getVelocity(this);

        al::normalize(&velocity);
        al::calcJumpInertia(&inertia, this, velocity, 0.7f);
        al::addVelocity(this, inertia);
    }

    applyInertia(this);

    if (al::isOnGround(this, 0)) {
        al::setVelocityZero(this);
        al::startAction(this, "MoveEnd");
        al::setNerve(this, &MoveEnd);
    }
}

void Popn::exeMoveEnd() {
    applyInertia(this);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPopn.Turn);
}

void Popn::exeBlowDown() {
    if (al::isFirstStep(this))
        al::onCollide(this);

    if (al::updateNerveState(this)) {
        al::startHitReaction(this, "死亡");
        if (mIsGenerateItem)
            al::appearItem(this);
        al::validateClipping(this);
        kill();
    }
}
