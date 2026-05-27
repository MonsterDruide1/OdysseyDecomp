#include "Enemy/TRexStateSleep.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(TRexStateSleep, Sleep);
NERVE_IMPL(TRexStateSleep, SleepReactionHipDrop);

NERVES_MAKE_NOSTRUCT(TRexStateSleep, Sleep, SleepReactionHipDrop);
}  // namespace

TRexStateSleep::TRexStateSleep(const char* name, al::LiveActor* actor)
    : al::ActorStateBase(name, actor) {
    initNerve(&Sleep, 0);
}

void TRexStateSleep::appear() {
    al::ActorStateBase::appear();
    al::offCollide(mActor);
    al::setNerve(this, &Sleep);
}

bool TRexStateSleep::receiveMsg(bool* outResult, const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isSensorCollision(self) && rs::isMsgPlayerAndCapHipDropAll(message)) {
        *outResult = false;
        al::setNerve(this, &SleepReactionHipDrop);
        return true;
    }

    return false;
}

void TRexStateSleep::exeSleep() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(mActor, "Sleep");
        al::setVelocityZero(mActor);
    }
}

void TRexStateSleep::exeSleepReactionHipDrop() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "SleepReactionHipDrop");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &Sleep);
}
