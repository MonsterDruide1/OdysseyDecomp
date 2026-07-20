#include "MapObj/SequentialSwitch.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

SequentialSwitch::SequentialSwitch(const char* name) : al::LiveActor(name) {}

void SequentialSwitch::init(const al::ActorInitInfo& info) {
    using SequentialSwitchFunctor = al::FunctorV0M<SequentialSwitch*, void (SequentialSwitch::*)()>;

    al::initActorSceneInfo(this, info);
    al::initStageSwitch(this, info);
    al::listenStageSwitchOn(this, "InputSwitchA",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    al::listenStageSwitchOn(this, "InputSwitchB",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    al::listenStageSwitchOn(this, "InputSwitchC",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    al::listenStageSwitchOn(this, "InputSwitchD",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    al::listenStageSwitchOn(this, "InputSwitchE",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    al::listenStageSwitchOn(this, "InputSwitchF",
                            SequentialSwitchFunctor(this, &SequentialSwitch::notifyInputSwitchOn));
    makeActorDead();
}

void SequentialSwitch::notifyInputSwitchOn() {
    al::StringTmp<128> switchName("OutputSwitch%dOn", ++mInputSwitchOnCount);
    al::tryOnStageSwitch(this, switchName.cstr());
}
