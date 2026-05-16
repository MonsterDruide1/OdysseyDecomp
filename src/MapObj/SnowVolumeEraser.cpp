#include "MapObj/SnowVolumeEraser.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(SnowVolumeEraser, Appear)

NERVES_MAKE_NOSTRUCT(SnowVolumeEraser, Appear)
}  // namespace

SnowVolumeEraser::SnowVolumeEraser(const char* actorName) : al::LiveActor(actorName) {}

void SnowVolumeEraser::init(const al::ActorInitInfo& info) {
    using SnowVolumeEraserFunctor = al::FunctorV0M<SnowVolumeEraser*, void (SnowVolumeEraser::*)()>;

    al::initActor(this, info);
    al::initNerve(this, &Appear, 0);

    if (al::listenStageSwitchOnStart(this, SnowVolumeEraserFunctor(this, &SnowVolumeEraser::start)))
        makeActorDead();
    else
        makeActorAlive();

    al::setSensorRadius(this, al::getScaleX(this) * 100.0f);
}

void SnowVolumeEraser::start() {
    makeActorAlive();
    al::setNerve(this, &Appear);
}

bool SnowVolumeEraser::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;
    return false;
}

void SnowVolumeEraser::exeAppear() {
    if (al::isGreaterEqualStep(this, 2))
        makeActorDead();
}
