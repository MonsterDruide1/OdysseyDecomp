#include "MapObj/DoorCity.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "System/GameDataFunction.h"

namespace {
NERVE_IMPL(DoorCity, Open);
NERVE_IMPL(DoorCity, WaitOpen);
NERVE_IMPL(DoorCity, WaitClose);

NERVES_MAKE_NOSTRUCT(DoorCity, Open, WaitOpen, WaitClose);
}  // namespace

DoorCity::DoorCity(const char* name) : al::LiveActor(name) {}

void DoorCity::init(const al::ActorInitInfo& info) {
    using DoorCityFunctor = al::FunctorV0M<DoorCity*, void (DoorCity::*)()>;

    const char* initState;

    al::initActor(this, info);
    initState = nullptr;
    al::getStringArg(&initState, info, "InitState");

    if (al::isEqualString(initState, "Open")) {
        al::initNerve(this, &WaitOpen, 0);
        al::invalidateCollisionParts(this);
    } else if (al::isEqualString(initState, "Close")) {
        al::initNerve(this, &WaitClose, 0);
        DoorCityFunctor onStageSwitchFunctor(this, &DoorCity::onStageSwitch);
        al::listenStageSwitchOnStart(this, onStageSwitchFunctor);
    } else {
        kill();
        return;
    }

    makeActorAlive();

    DoorCityFunctor listenSwitchOffFunctor(this, &DoorCity::listenSwitchOff);
    al::listenStageSwitchOn(this, "SwitchLightOff", listenSwitchOffFunctor);
}

void DoorCity::onStageSwitch() {
    al::setNerve(this, &Open);
}

void DoorCity::listenSwitchOff() {}

void DoorCity::exeWaitOpen() {
    if (al::isFirstStep(this))
        al::startAction(this, "OpenWait");
}

void DoorCity::exeOpen() {
    if (al::isFirstStep(this))
        al::startAction(this, "Open");

    if (al::isActionEnd(this)) {
        al::invalidateCollisionParts(this);
        al::setNerve(this, &WaitOpen);
    }
}

void DoorCity::exeWaitClose() {
    if (al::isFirstStep(this)) {
        if (GameDataFunction::getScenarioNo(this) == 1)
            al::tryStartAction(this, "Blink");
        else
            al::tryStartAction(this, "CloseWait");
    }
}
