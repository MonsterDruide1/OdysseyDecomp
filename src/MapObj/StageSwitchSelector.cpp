#include "MapObj/StageSwitchSelector.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/AppearSwitchSave.h"

namespace {
NERVE_IMPL(StageSwitchSelector, WaitStatUp)
NERVE_IMPL(StageSwitchSelector, WaitStartSwitching)
NERVE_IMPL(StageSwitchSelector, WaitDelaySwitching)

NERVES_MAKE_NOSTRUCT(StageSwitchSelector, WaitDelaySwitching)
NERVES_MAKE_STRUCT(StageSwitchSelector, WaitStatUp, WaitStartSwitching)
}  // namespace

StageSwitchSelector::StageSwitchSelector(const char* name) : al::LiveActor(name) {}

void StageSwitchSelector::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "監視オブジェ");
    al::initStageSwitch(this, info);
    al::initActorPoseTRSV(this);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);

    mAppearSwitchSave = new AppearSwitchSave(this, info);

    if (mAppearSwitchSave->isOn()) {
        al::tryOnStageSwitch(this, "AfterSwitchingOn");
        makeActorDead();
        return;
    }

    al::tryGetArg(&mSwitchingStartFrameNum, info, "SwitchingStartFrameNum");
    if (al::isValidStageSwitch(this, "SwitchStart")) {
        al::initNerve(this, &NrvStageSwitchSelector.WaitStatUp, 0);
    } else {
        al::tryOnStageSwitch(this, "BeforeSwitchingOn");
        al::initNerve(this, &NrvStageSwitchSelector.WaitStartSwitching, 0);
    }
    makeActorAlive();
}

void StageSwitchSelector::exeWaitStatUp() {
    if (al::isOnStageSwitch(this, "SwitchStart")) {
        al::tryOnStageSwitch(this, "BeforeSwitchingOn");
        al::setNerve(this, &NrvStageSwitchSelector.WaitStartSwitching);
    }
}

void StageSwitchSelector::exeWaitStartSwitching() {
    if (al::isOnStageSwitch(this, "StartSwitching"))
        al::setNerve(this, &WaitDelaySwitching);
}

void StageSwitchSelector::exeWaitDelaySwitching() {
    if (al::isFirstStep(this))
        mSwitchingDelay = mSwitchingStartFrameNum;

    if (mSwitchingDelay != 0)
        mSwitchingDelay--;
    else {
        al::tryOffStageSwitch(this, "BeforeSwitchingOn");
        al::tryOnStageSwitch(this, "AfterSwitchingOn");
        mAppearSwitchSave->onSwitch();
        makeActorDead();
    }
}
