#pragma once

namespace al {
class IUseStageSwitch;
class StageSwitchDirector;
class PlacementInfo;

void initStageSwitch(IUseStageSwitch* user, StageSwitchDirector* stageSwitchDirector,
                     const PlacementInfo& placementInfo);
bool isValidStageSwitch(const IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(const IUseStageSwitch* user, const char* linkName);
void onStageSwitch(IUseStageSwitch* user, const char* linkName);
void offStageSwitch(IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(IUseStageSwitch* user, const char* linkName);
bool isOffStageSwitch(IUseStageSwitch* user, const char* linkName);
bool isSameStageSwitch(const IUseStageSwitch* user, const IUseStageSwitch* otherUser,
                       const char* linkName);
bool isValidSwitchAppear(const IUseStageSwitch* user);
bool isOnSwitchAppear(const IUseStageSwitch* user);
bool isValidSwitchKill(const IUseStageSwitch* user);
bool isValidSwitchDeadOn(const IUseStageSwitch* user);
void onSwitchDeadOn(IUseStageSwitch* user);
void offSwitchDeadOn(IUseStageSwitch* user);
bool isOnSwitchDeadOn(IUseStageSwitch* user);
bool isOffSwitchDeadOn(IUseStageSwitch* user);
bool isValidSwitchStart(const IUseStageSwitch* user);
bool isOnSwitchStart(const IUseStageSwitch* user);
}  // namespace al
