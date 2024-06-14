#include "MapObj/BarrierField.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"

#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/StageSceneFunction.h"

namespace alNerveFunction {
class NerveActionCollector;
}

namespace {
NERVE_IMPL(BarrierField, Appear);
NERVE_IMPL(BarrierField, AppearBreedaMoonWorld);
NERVE_IMPL(BarrierField, Hide);
NERVE_IMPL(BarrierField, Disappear);

struct {
    NERVE_MAKE(BarrierField, Appear);
    NERVE_MAKE(BarrierField, AppearBreedaMoonWorld);
    NERVE_MAKE(BarrierField, Hide);
    NERVE_MAKE(BarrierField, Disappear);
} NrvBarrierField;

}  // namespace

static alNerveFunction::NerveActionCollector sBarrierFieldNerveCollector;

BarrierField::BarrierField(const char* name) : al::LiveActor(name) {}

/*
void BarrierField::init(const al::ActorInitInfo& initInfo) {
    al::initNerveAction(this, "Hide", &sBarrierFieldNerveCollector, 0);
    al::initMapPartsActor(this, initInfo, nullptr);
    al::tryGetArg(&mIsDisappearByShineGet, initInfo, "IsDisappearByShineGet");
    mIsOnTheMoon = al::isObjectName(initInfo, "WaterfallWorldHomeBarrier") &&
GameDataFunction::isWorldMoon(GameDataHolderAccessor(this));
    al::trySyncStageSwitchAppearAndKill(this);
}
*/

// NON_MATCHING

void BarrierField::init(const al::ActorInitInfo& initInfo) {
    al::initNerveAction(this, NrvBarrierField.Hide.getActionName(), &sBarrierFieldNerveCollector,
                        0);
    al::initMapPartsActor(this, initInfo, nullptr);
    al::tryGetArg(&mIsDisappearByShineGet, initInfo, "IsDisappearByShineGet");
    if (al::isObjectName(initInfo, "WaterfallWorldHomeBarrier")) {
        GameDataHolderAccessor dataAccessor(this);
        if (GameDataFunction::isWorldMoon(dataAccessor))
            mIsOnTheMoon = true;
    }
    al::trySyncStageSwitchAppearAndKill(this);
}

void BarrierField::appear() {
    LiveActor::appear();

    if (mIsOnTheMoon)
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
