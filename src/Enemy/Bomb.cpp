#include "Enemy/Bomb.h"

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

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Bomb, Wait);
NERVE_IMPL(Bomb, Trampled);
NERVE_IMPL(Bomb, WhipHold);
NERVE_IMPL(Bomb, Explosion);
NERVE_IMPL(Bomb, Throw);

NERVES_MAKE_STRUCT(Bomb, Wait, Trampled, WhipHold, Explosion, Throw);
}  // namespace

Bomb::Bomb(const char* name) : al::LiveActor(name) {}

void Bomb::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "Bomb", nullptr);
    al::initNerve(this, &NrvBomb.Wait, 0);
    makeActorAlive();
}

void Bomb::appearWithMsg(const al::LiveActor* actor, const al::SensorMsg* message) {
    al::copyPose(this, actor);
    al::resetPosition(this);
    appear();

    if (al::isMsgPlayerTrample(message)) {
        al::setNerve(this, &NrvBomb.Trampled);
        return;
    }

    if (rs::isMsgWhipHold(message)) {
        rs::tryInitWhipTarget(message, al::getHitSensor(this, "Body"), nullptr);
        al::setNerve(this, &NrvBomb.WhipHold);
        return;
    }

    al::setNerve(this, &NrvBomb.Wait);
}

void Bomb::exeWait() {}

void Bomb::exeTrampled() {
    if (al::isFirstStep(this))
        al::startAction(this, "TrampledDefault");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvBomb.Wait);
}

void Bomb::exeWhipHold() {
    if (al::isFirstStep(this))
        al::startAction(this, "TrampledDefault");
}

void Bomb::exeThrow() {
    al::addVelocityToGravity(this, 1.5f);

    if (!al::isLessStep(this, 5) && al::isCollided(this))
        al::setNerve(this, &NrvBomb.Explosion);
}

void Bomb::exeExplosion() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(this);
        al::setVelocityZero(this);
        al::offCollide(this);
        al::startHitReaction(this, "爆発");
        al::invalidateHitSensors(this);
        al::validateHitSensor(this, "ExplosionPlayer");
    }

    kill();
}

void Bomb::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvBomb.Throw)) {
        if (al::isSensorName(self, "Body") && al::sendMsgExplosion(other, self, nullptr))
            al::setNerve(this, &NrvBomb.Explosion);
    } else if (al::isNerve(this, &NrvBomb.Explosion) && al::isSensorName(self, "ExplosionPlayer")) {
        al::sendMsgExplosion(other, self, nullptr);
    }
}

bool Bomb::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    sead::Vector3f whipThrowDir;

    if (!rs::isMsgWhipThrow(message) || !rs::tryGetWhipThrowDir(&whipThrowDir, message))
        return false;

    al::setVelocityToDirection(this, whipThrowDir, 40.0f);
    al::addVelocityToGravity(this, -35.0f);
    al::invalidateClipping(this);
    al::setNerve(this, &NrvBomb.Throw);
    return true;
}
