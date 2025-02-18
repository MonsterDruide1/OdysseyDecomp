#include "Enemy/Togezo.h"

#include "Library/Collision/Collider.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Enemy/EnemyStateWander.h"
#include "Util/ExternalForceKeeper.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Togezo, Wait)
NERVE_IMPL(Togezo, Wander)
NERVE_IMPL(Togezo, CapHit)
NERVE_IMPL(Togezo, BlowDown)
NERVE_IMPL(Togezo, Attack)
NERVE_IMPL(Togezo, Turn)
NERVE_IMPL(Togezo, Fall)
NERVE_IMPL(Togezo, Find)
NERVE_IMPL(Togezo, Chase)
NERVE_IMPL(Togezo, Land)

NERVES_MAKE_NOSTRUCT(Togezo, Land)
NERVES_MAKE_STRUCT(Togezo, Wait, Wander, CapHit, BlowDown, Attack, Turn, Fall, Find, Chase)
}  // namespace

Togezo::Togezo(const char* name) : al::LiveActor(name) {}

void Togezo::init(const al::ActorInitInfo& info) {
    using TogezoFunctor = al::FunctorV0M<Togezo*, void (Togezo::*)()>;

    al::initActorWithArchiveName(this, info, "Togezo", nullptr);

    if (al::isValidStageSwitch(this, "SwitchStart"))
        al::initNerve(this, &NrvTogezo.Wait, 1);
    else
        al::initNerve(this, &NrvTogezo.Wander, 1);

    mStateWander = new EnemyStateWander(this, "Walk");
    al::initNerveState(this, mStateWander, &NrvTogezo.Wander, "徘徊");

    mForceKeeper = new ExternalForceKeeper();

    if (al::listenStageSwitchOnAppear(this, TogezoFunctor(this, &Togezo::listenAppear)))
        makeActorDead();
    else
        makeActorAlive();
}

void Togezo::listenAppear() {
    appear();
}

bool Togezo::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgTargetMarkerPosition(message)) {
        rs::setMsgTargetMarkerPosition(message,
                                       al::getTrans(this) + sead::Vector3f(0.0f, 180.0f, 0.0f));
        return true;
    }

    if (rs::isMsgKillByShineGet(message)) {
        kill();
        return true;
    }

    if (rs::isMsgNpcScareByEnemy(message))
        return true;

    if (al::tryReceiveMsgPushAndAddVelocity(this, message, other, self, 1.0))
        return true;

    if (rs::isMsgCapReflect(message) && !al::isNerve(this, &NrvTogezo.BlowDown) &&
        mCapHitCooldown <= 0) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &NrvTogezo.CapHit);
        mCapPos = al::getSensorPos(other);
        mCapHitCooldown = 10;
        return true;
    }

    if ((rs::isMsgBlowDown(message) || rs::isMsgDonsukeAttack(message)) &&
        !al::isNerve(this, &NrvTogezo.BlowDown)) {
        al::setVelocityBlowAttackAndTurnToTarget(this, al::getActorTrans(other), 15.0f, 35.0f);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &NrvTogezo.BlowDown);
        return true;
    }

    if ((rs::isMsgPechoSpot(message) || rs::isMsgDamageBallAttack(message) ||
         al::isMsgPlayerFireBallAttack(message)) &&
        al::isSensorEnemyBody(self)) {
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        rs::requestHitReactionToAttacker(message, self, other);
        al::startHitReaction(this, "死亡");

        kill();
        return false;
    }

    return mForceKeeper->receiveMsg(message, other, self);
}

void Togezo::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvTogezo.BlowDown))
        return;

    if (al::sendMsgEnemyAttack(other, self)) {
        al::setNerve(this, &NrvTogezo.Attack);
        return;
    }

    rs::sendMsgPushToPlayer(other, self);

    if (al::isNerve(this, &NrvTogezo.BlowDown))
        return;

    if (!al::isNerve(this, &NrvTogezo.BlowDown) && !al::isNerve(this, &NrvTogezo.Attack) &&
        (al::sendMsgEnemyAttackNeedle(other, self) || al::sendMsgEnemyAttack(other, self)))
        al::setNerve(this, &NrvTogezo.Attack);
    else
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);
}

void Togezo::control() {
    if (al::isInDeathArea(this) || al::isCollidedFloorCode(this, "DamageFire") ||
        al::isCollidedFloorCode(this, "Needle") || al::isCollidedFloorCode(this, "Poison") ||
        al::isInWaterArea(this)) {
        al::startHitReaction(this, "死亡");
        al::tryAddRippleMiddle(this);
        kill();

        return;
    }

    if (mCapHitCooldown > 0)
        mCapHitCooldown--;

    sead::Vector3f calculatedForce = sead::Vector3f::zero;
    mForceKeeper->calcForce(&calculatedForce);

    mFuturePos += calculatedForce * 0.64f;
    mFuturePos *= 0.955f;

    mForceKeeper->reset();

    if (!al::isNearZero(calculatedForce, 0.001f)) {
        mWanderCooldown = 180;
        al::invalidateClipping(this);
    }

    if (mWanderCooldown > 0) {
        mWanderCooldown--;

        if (mWanderCooldown == 0 && al::isNerve(this, &NrvTogezo.Wander))
            al::validateClipping(this);
    }
}

void Togezo::updateCollider() {
    const sead::Vector3f& velocity = al::getVelocity(this);

    if (al::isNoCollide(this)) {
        *al::getTransPtr(this) += velocity;
        al::getActorCollider(this)->onInvalidate();
    } else if (al::isFallOrDamageCodeNextMove(this, (velocity + mFuturePos) * 1.5f, 50.0f,
                                              200.0f)) {
        *al::getTransPtr(this) +=
            al::getActorCollider(this)->collide((velocity + mFuturePos) * 1.5f);
    } else {
        sead::Vector3f result = al::getActorCollider(this)->collide(velocity + mFuturePos);
        *al::getTransPtr(this) += result;
    }
}

void Togezo::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setVelocityZero(this);
    }
    if (al::isValidStageSwitch(this, "SwitchStart") && al::isOnStageSwitch(this, "SwitchStart"))
        al::setNerve(this, &NrvTogezo.Wander);
}

void Togezo::exeWander() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Walk");
    }

    al::updateNerveState(this);

    bool isGrounded = al::isOnGround(this, 0);
    bool isNearPlayer = al::isNearPlayer(this, 1000.0f);

    if (isGrounded && isNearPlayer) {
        al::setNerve(this, &NrvTogezo.Turn);
    } else if (isGrounded) {
        mAirTime = 0;
        mGroundNormal = al::getOnGroundNormal(this, 0);
    } else {
        if (mAirTime++ >= 4)
            al::setNerve(this, &NrvTogezo.Fall);
    }
}

void Togezo::exeTurn() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Turn");
    }

    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, this);

    al::LiveActor* player = al::tryFindNearestPlayerActor(this);
    if (player) {
        if (al::isFaceToTargetDegreeH(this, al::getTrans(player), frontDir, 1.0f)) {
            al::setNerve(this, &NrvTogezo.Find);
            return;
        }
        al::turnToTarget(this, al::getTrans(player), 3.5f);
    }

    if (!al::isNearPlayer(this, 1300.0f)) {
        al::setNerve(this, &NrvTogezo.Wander);
        return;
    }

    if (al::isOnGround(this, 0)) {
        mAirTime = 0;
    } else {
        al::addVelocityToGravity(this, 1.0f);
        al::scaleVelocity(this, 0.98f);

        if (mAirTime++ >= 4)
            al::setNerve(this, &NrvTogezo.Fall);
    }
}

void applyTogezoGroundVelocity(Togezo* self) {
    if (al::isOnGround(self, 0)) {
        const sead::Vector3f& normal = al::getOnGroundNormal(self, 0);
        al::getVelocity(self);  // unused

        if (al::isFallOrDamageCodeNextMove(self, al::getVelocity(self), 50.0f, 200.0f)) {
            f32 y = al::getVelocity(self).y;
            al::scaleVelocity(self, -1.0f);
            al::getVelocityPtr(self)->y = y;
        } else {
            al::addVelocity(self, -normal);
            al::scaleVelocity(self, 0.95f);
        }
    } else {
        al::addVelocityY(self, -2.0f);
        al::scaleVelocity(self, 0.98f);
    }
}

void Togezo::exeFind() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Find");
        mAirTime = 0;
        al::invalidateClipping(this);
    }

    if (!al::isOnGround(this, 0) && mAirTime++ >= 4) {
        al::setNerve(this, &NrvTogezo.Fall);
    } else {
        applyTogezoGroundVelocity(this);
        if (al::isActionEnd(this))
            al::setNerve(this, &NrvTogezo.Chase);
    }
}

void Togezo::exeChase() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Run");
        al::invalidateClipping(this);
    }

    if (al::isOnGround(this, 0)) {
        sead::Vector3f normal = al::getOnGroundNormal(this, 0);
        al::scaleVelocityDirection(this, normal, 0);
        mAirTime = 0;

        al::LiveActor* player = al::tryFindNearestPlayerActor(this);
        if (player) {
            al::turnToTarget(this, al::getTrans(player), 3.5f);

            sead::Vector3f frontDir = sead::Vector3f::zero;
            al::calcFrontDir(&frontDir, this);

            sead::Vector3f verticalVel = sead::Vector3f::zero;
            sead::Vector3f horizontalVel = sead::Vector3f::zero;
            al::separateVectorHV(&horizontalVel, &verticalVel, normal, frontDir);
            al::tryNormalizeOrDirZ(&horizontalVel, horizontalVel);

            al::addVelocity(this, horizontalVel * 0.6f);
            al::scaleVelocity(this, 0.95f);
        }
        if (!al::isNearPlayer(this, 1300.0f)) {
            al::setNerve(this, &NrvTogezo.Wander);
            return;
        }
    } else if (mAirTime++ >= 4) {
        al::setNerve(this, &NrvTogezo.Fall);
        return;
    }

    applyTogezoGroundVelocity(this);
}

void Togezo::exeFall() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::startAction(this, "Fall");
    }

    applyTogezoGroundVelocity(this);

    if (al::isOnGround(this, 0)) {
        mAirTime = 0;
        al::validateClipping(this);
        al::setNerve(this, &Land);
    }
}

void Togezo::exeLand() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Land");
        mAirTime = 0;
    }

    s32* airTimePtr = &mAirTime;

    applyTogezoGroundVelocity(this);

    if (!al::isOnGround(this, 0) && (*airTimePtr)++ >= 4)
        al::setNerve(this, &NrvTogezo.Fall);
    else if (al::isActionEnd(this))
        al::setNerve(this, &NrvTogezo.Wander);
}

void Togezo::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "AttackSuccess");
        al::setVelocityZero(this);
    }

    applyTogezoGroundVelocity(this);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvTogezo.Wander);
}

void Togezo::exeCapHit() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "CapHit");

        sead::Vector3f capDirection = al::getTrans(this) - mCapPos;
        capDirection.y = 0.0f;

        al::tryNormalizeOrDirZ(&capDirection, capDirection);
        al::setVelocity(this, capDirection * 20.0f);

        sead::Quatf frontUp = sead::Quatf::unit;
        al::makeQuatUpFront(&frontUp, capDirection, sead::Vector3f::ey);

        mAirTime = 0;

        al::invalidateClipping(this);
    }

    if (al::isActionEnd(this)) {
        if (al::isNearPlayer(this, 1000.0f))
            al::setNerve(this, &NrvTogezo.Find);
        else
            al::setNerve(this, &NrvTogezo.Wander);
    } else if (al::isOnGround(this, 0)) {
        mAirTime = 0;

        al::addVelocityToGravity(this, 1.0);
        al::scaleVelocity(this, 0.95f);

        sead::Vector3f velocity = al::getVelocity(this);
        f32 y = velocity.y;
        velocity.y = 0.0f;

        if (al::tryNormalizeOrZero(&velocity, velocity)) {
            if (al::isFallOrDamageCodeNextMove(this, velocity * 10.0f, 50.0f, 200.0f))
                al::setVelocity(this,
                                sead::Vector3f(velocity * 5.0f + sead::Vector3f(0.0f, y, 0.0f)));
        }

    } else if (mAirTime++ >= 5) {
        al::setNerve(this, &NrvTogezo.Fall);
    } else {
        al::addVelocityToGravity(this, 1.0f);
        al::scaleVelocity(this, 0.98f);
    }
}

void Togezo::exeBlowDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "BlowDown");
        al::invalidateClipping(this);
    }

    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.98f);

    if (al::isActionEnd(this)) {
        al::startHitReaction(this, "死亡");
        al::appearItem(this);
        kill();
    }
}
