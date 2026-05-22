#include "MapObj/DelaySwitch.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace {
NERVE_IMPL(DelaySwitch, Run)

NERVES_MAKE_NOSTRUCT(DelaySwitch, Run)
}  // namespace

DelaySwitch::DelaySwitch(const char* name) : al::LiveActor(name) {}

void DelaySwitch::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initStageSwitch(this, info);
    al::initNerve(this, &Run, 0);
    al::tryGetArg(&mDelayFrame, info, "DelayFrame");
    al::tryListenStageSwitchAppear(this);
}

void DelaySwitch::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Run);
}

void DelaySwitch::exeRun() {
    if (al::isGreaterEqualStep(this, mDelayFrame - 1)) {
        al::tryOnStageSwitch(this, "OutputSwitchOn");
        makeActorDead();
    }
}
