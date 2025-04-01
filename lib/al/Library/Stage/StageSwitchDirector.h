#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class StageSwitchListener;
class PlacementId;
class StageSwitchAccesser;
class ExecuteDirector;
class StageSwitchWatcherHolder;
struct StageSwitchInfo;

class StageSwitchDirector : public HioNode, public IUseExecutor {
public:
    StageSwitchDirector(ExecuteDirector* executeDirector);

    s32 useSwitch(const StageSwitchAccesser* accesser);
    s32 findSwitchNoFromObjId(const PlacementId* placementId);
    void onSwitch(const StageSwitchAccesser* accesser);
    void offSwitch(const StageSwitchAccesser* accesser);
    bool isOnSwitch(const StageSwitchAccesser* accesser);
    void addListener(StageSwitchListener* listener, StageSwitchAccesser* accesser);
    void execute() override;

private:
    StageSwitchInfo* mSwitchInfos = nullptr;
    s32 mSwitchInfoCapacity = 128;
    s32 mSwitchInfoCount = 0;
    StageSwitchWatcherHolder* mSwitchWatcherHolder = nullptr;
};

static_assert(sizeof(StageSwitchDirector) == 0x20);

}  // namespace al
