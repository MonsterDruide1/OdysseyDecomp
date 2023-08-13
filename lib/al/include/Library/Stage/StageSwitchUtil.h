#pragma once

namespace al {
class IUseStageSwitch;
class StageSwitchDirector;
class PlacementInfo;

void initStageSwitch(al::IUseStageSwitch* user, al::StageSwitchDirector* stageSwitchDirector, const al::PlacementInfo& placementInfo);
bool isValidStageSwitch(const al::IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(const al::IUseStageSwitch* user, const char* linkName);
void onStageSwitch(al::IUseStageSwitch* user, const char* linkName);
void offStageSwitch(al::IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(al::IUseStageSwitch* user, const char* linkName);
bool isOffStageSwitch(al::IUseStageSwitch* user, const char* linkName);
bool isSameStageSwitch(const al::IUseStageSwitch* user, const al::IUseStageSwitch* otherUser, const char* linkName);
bool isValidSwitchAppear(const al::IUseStageSwitch* user);
bool isOnSwitchAppear(const al::IUseStageSwitch* user);
bool isValidSwitchKill(const al::IUseStageSwitch* user);
bool isValidSwitchDeadOn(const al::IUseStageSwitch* user);
void onSwitchDeadOn(al::IUseStageSwitch* user);
void offSwitchDeadOn(al::IUseStageSwitch* user);
bool isOnSwitchDeadOn(al::IUseStageSwitch* user);
bool isOffSwitchDeadOn(al::IUseStageSwitch* user);
bool isValidSwitchStart(const al::IUseStageSwitch* user);
bool isOnSwitchStart(const al::IUseStageSwitch* user);
}  // namespace al
