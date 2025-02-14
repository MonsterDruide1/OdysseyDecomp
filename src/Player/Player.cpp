#include "Player/Player.h"

#include <math/seadVector.h>

#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerFunction.h"

namespace {
NERVE_IMPL(Player, Wait);
NERVE_IMPL(Player, Run);
NERVE_IMPL(Player, Jump);
NERVE_IMPL(Player, Fall);
NERVE_IMPL(Player, Damage);

NERVES_MAKE_NOSTRUCT(Player, Damage);
NERVES_MAKE_STRUCT(Player, Fall, Jump, Run, Wait);
}  // namespace

Player::Player(const char* actorName, const char* archiveName, s32 port)
    : al::LiveActor(actorName), mArchiveName(archiveName), mPort(port) {}

void Player::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, nullptr);
    al::invalidateClipping(this);
    al::initNerve(this, &NrvPlayer.Fall, 0);
    makeActorAlive();
}

void Player::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    al::scaleVelocity(this, 0.7f);

    if (al::isPadTriggerA(mPort))
        al::setNerve(this, &NrvPlayer.Jump);
    else if (!al::isNearZero(al::getLeftStick(mPort), 0.001f))
        al::setNerve(this, &NrvPlayer.Run);
}

void Player::exeRun() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Run");
        mAirTime = 0;
    }
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.7f);
    al::addPlayerAccelStickGravity(this, al::getFrontPtr(this), 3.0f,
                                   sead::Vector3f{0.0f, 1.0f, 0.0f}, mPort, nullptr);
    al::setSklAnimFrameRate(this, al::calcSpeed(this) * 0.23f, 0);

    if (al::isPadTriggerA(mPort)) {
        al::setNerve(this, &NrvPlayer.Jump);
        return;
    }

    if (al::isCollidedGround(this)) {
        mAirTime = 0;
    } else {
        mAirTime += 1;
        if (mAirTime > 5) {
            al::setNerve(this, &NrvPlayer.Fall);
            return;
        }
    }
    if (al::isVelocitySlow(this, 1.0f)) {
        al::setNerve(this, &NrvPlayer.Wait);
        return;
    }

    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void Player::exeJump() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Jump");
        al::addVelocityJump(this, 28.0f);
    }
    al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, 0.99f);
    al::addPlayerAccelStickGravity(this, nullptr, 0.2f, sead::Vector3f{0.0f, 1.0f, 0.0f}, mPort,
                                   nullptr);

    bool onGround = al::isOnGround(this, 0);  // required for match
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    if (onGround)
        al::setNerve(this, &NrvPlayer.Wait);
}

void Player::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");
    al::addVelocityToGravity(this, 0.7f);
    al::scaleVelocity(this, 0.99f);
    if (al::isLessEqualStep(this, 10) && al::isPadTriggerA(mPort)) {
        al::setNerve(this, &NrvPlayer.Jump);
        return;
    }

    if (al::isOnGround(this, 0)) {
        al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        al::setNerve(this, &NrvPlayer.Wait);
    }
}

void Player::exeDamage() {
    if ((al::isFirstStep(this)))
        al::startAction(this, "Damage");
    al::addVelocityToGravity(this, 0.7f);
    al::scaleVelocity(this, 0.99f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    if (al::isStep(this, 60)) {
        al::setVelocityZero(this);
        al::setNerve(this, &NrvPlayer.Fall);
    }
}

void Player::control() {
    if (al::isPadTriggerL(mPort)) {
        al::setVelocityZero(this);
        al::setTrans(this, {0.0f, 100.0f, 800.0f});
        al::resetPosition(this);
        al::setNerve(this, &NrvPlayer.Fall);
    }
}

// NON_MATCHING: issue with getting actorTrans
void Player::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    const sead::Vector3f& actorTransRef = al::getActorTrans(other);
    const sead::Vector3f& transRef = al::getTrans(this);
    sead::Vector3f actorTrans = actorTransRef;
    sead::Vector3f trans = transRef;

    if ((al::isNerve(this, &NrvPlayer.Jump) || al::isNerve(this, &NrvPlayer.Fall)) &&
        (al::isSensorEnemy(other) || al::isSensorPlayer(other)) &&
        (al::getVelocity(this).dot(al::getGravity(this)) > 0.0f) &&
        ((actorTrans - trans).dot(al::getGravity(this)) > 0.0f) &&
        al::sendMsgPlayerAttackTrample(other, self, nullptr))
        al::setVelocityJump(this, 23.0f);
}

bool Player::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerTrample(message) || al::isMsgEnemyAttack(message)) {
        if (!al::isNerve(this, &Damage)) {
            sead::Vector3f offset = al::getTrans(this) - al::getSensorPos(other);
            al::verticalizeVec(&offset, al::getGravity(this), offset);
            al::tryNormalizeOrZero(&offset);

            if (!al::isNearZero(offset, 0.001f)) {
                sead::Vector3f* front = al::getFrontPtr(this);
                front->set(offset);
            }

            sead::Vector3f velocity = offset * 10.0f;
            al::setVelocity(this, velocity);

            al::addVelocityJump(this, 20.0f);
            al::setNerve(this, &Damage);
            return true;
        }
    }
    return false;
}
