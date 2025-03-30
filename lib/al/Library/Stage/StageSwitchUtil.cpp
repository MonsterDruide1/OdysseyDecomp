#include "Library/Stage/StageSwitchUtil.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/IUseStageSwitch.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"
#include "Project/Stage/StageSwitchAccesser.h"
#include "Project/Stage/StageSwitchFunctorListener.h"

namespace al {
void initStageSwitch(IUseStageSwitch* user, StageSwitchDirector* stageSwitchDirector,
                     const PlacementInfo& placementInfo) {
    if (user->getStageSwitchKeeper())
        return;

    user->initStageSwitchKeeper();

    s32 linkCount = calcLinkCountClassName(placementInfo, "StageSwitch");
    if (linkCount == 0)
        return;

    StageSwitchKeeper* keeper = user->getStageSwitchKeeper();
    keeper->setUseName(user);
    keeper->init(stageSwitchDirector, placementInfo);
}

static StageSwitchAccesser* getStageSwitchAccesser(const IUseStageSwitch* user,
                                                   const char* linkName) {
    StageSwitchKeeper* keeper = user->getStageSwitchKeeper();
    if (!keeper)
        return nullptr;
    StageSwitchAccesser* accesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (!accesser)
        return nullptr;

    accesser->isEnableRead();
    return accesser;
}

bool isValidStageSwitch(const IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    return accesser && accesser->isValid();
}

bool isOnStageSwitch(const IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    return accesser && accesser->isOnSwitch();
}

void onStageSwitch(IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    if (accesser)
        accesser->onSwitch();
}

void offStageSwitch(IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    if (accesser)
        accesser->offSwitch();
}

bool tryOnStageSwitch(IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    if (!accesser || !accesser->isValid() || accesser->isOnSwitch())
        return false;
    accesser->onSwitch();
    return true;
}

bool tryOffStageSwitch(IUseStageSwitch* user, const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    if (!accesser || !accesser->isValid() || !accesser->isOnSwitch())
        return false;
    accesser->offSwitch();
    return true;
}

bool isSameStageSwitch(const IUseStageSwitch* user, const IUseStageSwitch* otherUser,
                       const char* linkName) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, linkName);
    if (!accesser)
        return false;

    StageSwitchAccesser* otherAccesser = getStageSwitchAccesser(otherUser, linkName);
    if (!otherAccesser)
        return false;

    return accesser->isEqualSwitch(otherAccesser);
}

bool isValidSwitchAppear(const IUseStageSwitch* user) {
    return isValidStageSwitch(user, "SwitchAppear");
}

bool isOnSwitchAppear(const IUseStageSwitch* user) {
    return isOnStageSwitch(user, "SwitchAppear");
}

bool isValidSwitchKill(const IUseStageSwitch* user) {
    return isValidStageSwitch(user, "SwitchKill");
}

bool isValidSwitchDeadOn(const IUseStageSwitch* user) {
    return isValidStageSwitch(user, "SwitchDeadOn");
}

void onSwitchDeadOn(IUseStageSwitch* user) {
    return onStageSwitch(user, "SwitchDeadOn");
}

void offSwitchDeadOn(IUseStageSwitch* user) {
    return offStageSwitch(user, "SwitchDeadOn");
}

bool tryOnSwitchDeadOn(IUseStageSwitch* user) {
    return tryOnStageSwitch(user, "SwitchDeadOn");
}

bool tryOffSwitchDeadOn(IUseStageSwitch* user) {
    return tryOffStageSwitch(user, "SwitchDeadOn");
}

bool isValidSwitchStart(const IUseStageSwitch* user) {
    return isValidStageSwitch(user, "SwitchStart");
}

bool isOnSwitchStart(const IUseStageSwitch* user) {
    return isOnStageSwitch(user, "SwitchStart");
}

bool listenStageSwitchOn(IUseStageSwitch* user, const char* eventName, const FunctorBase& action) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, eventName);
    if (!accesser || !accesser->isValid())
        return false;

    auto* listener = new StageSwitchFunctorListener();
    listener->setOnFunctor(action);
    accesser->addListener(listener);
    return true;
}

bool listenStageSwitchOff(IUseStageSwitch* user, const char* eventName, const FunctorBase& action) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, eventName);
    if (!accesser || !accesser->isValid())
        return false;

    auto* listener = new StageSwitchFunctorListener();
    listener->setOffFunctor(action);
    accesser->addListener(listener);
    return true;
}

bool listenStageSwitchOnOff(IUseStageSwitch* user, const char* eventName,
                            const FunctorBase& actionOn, const FunctorBase& actionOff) {
    StageSwitchAccesser* accesser = getStageSwitchAccesser(user, eventName);
    if (!accesser || !accesser->isValid())
        return false;

    auto* listener = new StageSwitchFunctorListener();
    listener->setOnFunctor(actionOn);
    listener->setOffFunctor(actionOff);
    accesser->addListener(listener);
    return true;
}

bool listenStageSwitchOnAppear(IUseStageSwitch* user, const FunctorBase& action) {
    return listenStageSwitchOn(user, "SwitchAppear", action);
}

bool listenStageSwitchOnOffAppear(IUseStageSwitch* user, const FunctorBase& actionOn,
                                  const FunctorBase& actionOff) {
    return listenStageSwitchOnOff(user, "SwitchStart", actionOn, actionOff);
}

bool listenStageSwitchOnKill(IUseStageSwitch* user, const FunctorBase& action) {
    return listenStageSwitchOn(user, "SwitchKill", action);
}

bool listenStageSwitchOnOffKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                const FunctorBase& actionOff) {
    return listenStageSwitchOnOff(user, "SwitchKill", actionOn, actionOff);
}

bool trySyncStageSwitchOnOffAppear(IUseStageSwitch* user, const FunctorBase& actionOn,
                                   const FunctorBase& actionOff) {
    if (listenStageSwitchOnOffAppear(user, actionOn, actionOff)) {
        actionOff();
        return true;
    } else {
        actionOn();
        return false;
    }
}

bool trySyncStageSwitchOnOffKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                 const FunctorBase& actionOff) {
    bool listened = listenStageSwitchOnOffKill(user, actionOn, actionOff);
    actionOff();
    return listened;
}

bool trySyncStageSwitchOnOffAppearAndKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                          const FunctorBase& actionOff) {
    if (trySyncStageSwitchOnOffAppear(user, actionOn, actionOff))
        return true;

    return trySyncStageSwitchOnOffKill(user, actionOff, actionOn);
}

bool listenStageSwitchOnStart(IUseStageSwitch* user, const FunctorBase& action) {
    return listenStageSwitchOn(user, "SwitchStart", action);
}

bool listenStageSwitchOnOffStart(IUseStageSwitch* user, const FunctorBase& actionOn,
                                 const FunctorBase& actionOff) {
    return listenStageSwitchOnOff(user, "SwitchStart", actionOn, actionOff);
}

bool listenStageSwitchOnStop(IUseStageSwitch* user, const FunctorBase& action) {
    return listenStageSwitchOn(user, "SwitchStop", action);
}

}  // namespace al
