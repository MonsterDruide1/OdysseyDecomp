#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"
#include "Library/HostIO/IUseName.h"

namespace al {
class ExecuteDirector;
class PlacementId;
class StageSwitchAccesser;
class StageSwitchDirector;
class StageSwitchListener;
class PlacementInfo;

class StageSwitchKeeper {
public:
    StageSwitchKeeper();
    void init(StageSwitchDirector*, const PlacementInfo&);
    StageSwitchAccesser* tryGetStageSwitchAccesser(const char* name);

private:
    StageSwitchAccesser* mStageSwitchAccessers;
    s32 mAccesserNum;
    void* field_10;
};

class StageSwitchDirector : public HioNode, public IUseExecutor {
public:
    StageSwitchDirector(ExecuteDirector*);

    void execute() override;

    void useSwitch(const StageSwitchAccesser*);
    void* findSwitchNoFromObjId(const PlacementId*);  // unknown return type
    void onSwitch(const StageSwitchAccesser*);
    void offSwitch(const StageSwitchAccesser*);
    bool isOnSwitch(const StageSwitchAccesser*);
    void addListener(StageSwitchListener*, StageSwitchAccesser*);

private:
    void* filler[3];
};
static_assert(sizeof(StageSwitchDirector) == 0x20);

}  // namespace al
