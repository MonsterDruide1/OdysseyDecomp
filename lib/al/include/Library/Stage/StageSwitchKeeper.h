#pragma once

#include <basis/seadTypes.h>
#include "Library/HostIO/IUseName.h"

namespace al {
class StageSwitchAccesser;
class StageSwitchDirector;
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
}  // namespace al
