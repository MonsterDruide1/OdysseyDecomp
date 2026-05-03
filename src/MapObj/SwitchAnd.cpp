#include "MapObj/SwitchAnd.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

SwitchAnd::SwitchAnd(const char* name) : al::LiveActor(name) {}

void SwitchAnd::init(const al::ActorInitInfo& info) {
    using SwitchAndFunctor = al::FunctorV0M<SwitchAnd*, void (SwitchAnd::*)()>;

    al::initActorSceneInfo(this, info);
    al::initStageSwitch(this, info);

    if (al::listenStageSwitchOnOff(this, "InputSwitchA",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchB",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchC",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchD",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchE",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchF",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    makeActorDead();
}

void SwitchAnd::notifyInputSwitchOn() {
    mInputSwitchNum--;
    if (mInputSwitchNum == 0)
        al::tryOnStageSwitch(this, "OutputSwitchOn");
}

void SwitchAnd::notifyInputSwitchOff() {
    mInputSwitchNum++;
    if (mInputSwitchNum != 0)
        al::tryOffStageSwitch(this, "OutputSwitchOn");
}
