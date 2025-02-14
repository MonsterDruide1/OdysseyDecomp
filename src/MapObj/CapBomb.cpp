#include "MapObj/CapBomb.h"

#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CapBomb, Wait);
NERVE_IMPL(CapBomb, Explosion);

NERVES_MAKE_STRUCT(CapBomb, Wait, Explosion);
}  // namespace

CapBomb::CapBomb(const char* name) : al::LiveActor(name) {}

void CapBomb::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "Bomb", nullptr);
    al::initNerve(this, &NrvCapBomb.Wait, 0);
    al::invalidateHitSensors(this);
    al::validateHitSensor(this, "Body");
    al::hideSilhouetteModel(this);
    makeActorAlive();

    mTestLayout = new al::LayoutActor("テストレイアウト");
    al::initLayoutActor(mTestLayout, al::getLayoutInitInfo(info), "TestSugawaraLayout", nullptr);

    mTestLayoutParts = new al::LayoutActor("テストパーツ");
    al::initLayoutPartsActor(mTestLayoutParts, mTestLayout, al::getLayoutInitInfo(info), "Par_00",
                             nullptr);
    mTestLayout->kill();
}

void CapBomb::appear() {
    al::showModelIfHide(this);
    al::setVelocityZero(this);
    al::onCollide(this);
    al::invalidateHitSensors(this);
    al::validateHitSensor(this, "Body");

    al::setNerve(this, &NrvCapBomb.Wait);

    al::LiveActor::appear();
}

bool CapBomb::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isNerve(this, &NrvCapBomb.Wait) &&
        (al::isMsgPlayerObjTouch(message) || rs::isMsgItemGetByWeapon(message) ||
         al::isMsgExplosion(message))) {
        al::setNerve(this, &NrvCapBomb.Explosion);
        return true;
    }

    return false;
}

void CapBomb::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvCapBomb.Explosion))
        al::sendMsgExplosion(other, self, nullptr);
}

void CapBomb::exeWait() {}

void CapBomb::exeExplosion() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(this);
        al::setVelocityZero(this);
        al::offCollide(this);
        al::startHitReaction(this, "爆発");
        al::validateHitSensor(this, "ExplosionPlayer");
    } else
        kill();
}
