#pragma once

namespace al {
class FunctorBase;
class IUseStageSwitch;
class StageSwitchDirector;
class PlacementInfo;

void initStageSwitch(IUseStageSwitch* user, StageSwitchDirector* stageSwitchDirector,
                     const PlacementInfo& placementInfo);
bool isValidStageSwitch(const IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(const IUseStageSwitch* user, const char* linkName);
void onStageSwitch(IUseStageSwitch* user, const char* linkName);
void offStageSwitch(IUseStageSwitch* user, const char* linkName);
bool tryOnStageSwitch(IUseStageSwitch* user, const char* linkName);
bool tryOffStageSwitch(IUseStageSwitch* user, const char* linkName);
bool isSameStageSwitch(const IUseStageSwitch* user, const IUseStageSwitch* otherUser,
                       const char* linkName);
bool isValidSwitchAppear(const IUseStageSwitch* user);
bool isOnSwitchAppear(const IUseStageSwitch* user);
bool isValidSwitchKill(const IUseStageSwitch* user);
bool isValidSwitchDeadOn(const IUseStageSwitch* user);
void onSwitchDeadOn(IUseStageSwitch* user);
void offSwitchDeadOn(IUseStageSwitch* user);
bool tryOnSwitchDeadOn(IUseStageSwitch* user);
bool tryOffSwitchDeadOn(IUseStageSwitch* user);
bool isValidSwitchStart(const IUseStageSwitch* user);
bool isOnSwitchStart(const IUseStageSwitch* user);
bool listenStageSwitchOn(IUseStageSwitch* user, const char* eventName, const FunctorBase& action);
bool listenStageSwitchOff(IUseStageSwitch* user, const char* eventName, const FunctorBase& action);
bool listenStageSwitchOnOff(IUseStageSwitch* user, const char* eventName,
                            const FunctorBase& actionOn, const FunctorBase& actionOff);
bool listenStageSwitchOnAppear(IUseStageSwitch* user, const FunctorBase& action);
bool listenStageSwitchOnOffAppear(IUseStageSwitch* user, const FunctorBase& actionOn,
                                  const FunctorBase& actionOff);
bool listenStageSwitchOnKill(IUseStageSwitch* user, const FunctorBase& action);
bool listenStageSwitchOnOffKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                const FunctorBase& actionOff);
bool trySyncStageSwitchOnOffAppear(IUseStageSwitch* user, const FunctorBase& actionOn,
                                   const FunctorBase& actionOff);
bool trySyncStageSwitchOnOffKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                 const FunctorBase& actionOff);
bool trySyncStageSwitchOnOffAppearAndKill(IUseStageSwitch* user, const FunctorBase& actionOn,
                                          const FunctorBase& actionOff);
bool trySyncStageSwitchOnOff(IUseStageSwitch* user, const FunctorBase& actionOn,
                             const FunctorBase& actionOff);
bool listenStageSwitchOnStart(IUseStageSwitch* user, const FunctorBase& action);
bool listenStageSwitchOnOffStart(IUseStageSwitch* user, const FunctorBase& actionOn,
                                 const FunctorBase& actionOff);
bool listenStageSwitchOnStop(IUseStageSwitch* user, const FunctorBase& action);
}  // namespace al
