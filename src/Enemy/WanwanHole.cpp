#include "Enemy/WanwanHole.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(WanwanHole, Wait);
NERVE_IMPL(WanwanHole, In);
NERVES_MAKE_NOSTRUCT(WanwanHole, Wait, In);
}  // namespace

WanwanHole::WanwanHole(const char* name) : al::LiveActor(name) {}

void WanwanHole::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &Wait, 0);
    f32 addHoleRadius = 0.0f;
    al::tryGetArg(&addHoleRadius, initInfo, "AddHoleRadius");
    al::setSensorRadius(this, addHoleRadius);
    makeActorAlive();
}

void WanwanHole::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &Wait)) {
        if (rs::sendMsgWanwanHoleIn(other, self)) {
            al::invalidateClipping(this);
            al::setNerve(this, &In);
        }
    }
}

bool WanwanHole::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;
    return false;
}

void WanwanHole::exeWait() {
    if (al::isFirstStep(this))
        al::validateClipping(this);
}

void WanwanHole::exeIn() {
    if (al::isGreaterEqualStep(this, 60))
        al::tryOnStageSwitch(this, "SwitchWanwanInOn");
    if (al::isGreaterEqualStep(this, 100))
        al::setNerve(this, &Wait);
}
