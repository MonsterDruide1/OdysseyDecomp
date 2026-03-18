#include "Boss/Koopa/KoopaFireBeamGround.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(KoopaFireBeamGround, AppearSign)
NERVE_IMPL(KoopaFireBeamGround, Wait)

NERVES_MAKE_NOSTRUCT(KoopaFireBeamGround, AppearSign, Wait)
}  // namespace

KoopaFireBeamGround::KoopaFireBeamGround(const char* name) : al::LiveActor(name) {}

void KoopaFireBeamGround::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "KoopaFireBeamGround", nullptr);
    al::initNerve(this, &AppearSign, 0);
    makeActorDead();
}

void KoopaFireBeamGround::appearSign() {
    al::LiveActor::appear();
    al::setNerve(this, &AppearSign);
}

void KoopaFireBeamGround::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Wait);
}

void KoopaFireBeamGround::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &AppearSign))
        return;
    if (!rs::isPlayerOnGround(this))
        return;
    al::sendMsgEnemyAttackFire(other, self, nullptr);
}

bool KoopaFireBeamGround::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                     al::HitSensor* self) {
    return al::isMsgPlayerDisregard(message);
}

void KoopaFireBeamGround::exeAppearSign() {
    if (al::isFirstStep(this))
        al::startAction(this, "AppearSign");
}

void KoopaFireBeamGround::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}
