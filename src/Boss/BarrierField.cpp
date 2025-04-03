#include "Boss/BarrierField.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "System/GameDataFunction.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_ACTION_IMPL(BarrierField, Appear);
NERVE_ACTION_IMPL(BarrierField, AppearBreedaMoonWorld);
NERVE_ACTION_IMPL(BarrierField, Hide);
NERVE_ACTION_IMPL(BarrierField, Disappear);

NERVE_ACTIONS_MAKE_STRUCT(BarrierField, Appear, AppearBreedaMoonWorld, Hide, Disappear);

}  // namespace

BarrierField::BarrierField(const char* name) : al::LiveActor(name) {}

void BarrierField::init(const al::ActorInitInfo& initInfo) {
    al::initNerveAction(this, "Hide", &NrvBarrierField.collector, 0);
    al::initMapPartsActor(this, initInfo, nullptr);
    al::tryGetArg(&mIsDisappearByShineGet, initInfo, "IsDisappearByShineGet");
    if (al::isObjectName(initInfo, "WaterfallWorldHomeBarrier") &&
        GameDataFunction::isWorldMoon(this))
        mIsMoon = true;
    al::trySyncStageSwitchAppearAndKill(this);
}

void BarrierField::appear() {
    LiveActor::appear();

    if (mIsMoon)
        al::startNerveAction(this, "AppearBreedaMoonWorld");
    else
        al::startNerveAction(this, "Appear");

    al::tryOnStageSwitch(this, "SwitchExistOnOff");
    al::invalidateClipping(this);
    if (mIsDisappearByShineGet)
        rs::setBossBarrierField(this);
}

void BarrierField::kill() {
    al::tryOffStageSwitch(this, "SwitchExistOnOff");
    al::validateClipping(this);
    al::LiveActor::kill();
}

void BarrierField::disappearByShineGet() {
    if (getEffectKeeper())
        al::tryKillEmitterAndParticleAll(this);
    kill();
}

void BarrierField::exeAppear() {}

void BarrierField::exeAppearBreedaMoonWorld() {}

void BarrierField::exeHide() {}

void BarrierField::exeDisappear() {
    kill();
}
