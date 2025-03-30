#pragma once

#include <basis/seadTypes.h>

namespace al {
class ExecuteDirector;
class PlacementId;
class StageSwitchAccesser;
class StageSwitchDirector;
class StageSwitchListener;
class PlacementInfo;
class IUseStageSwitch;
class FunctorBase;
class IUseName;

struct StageSwitchAccesserList {
    StageSwitchAccesserList();
    StageSwitchAccesserList(const StageSwitchAccesser* accessers);

    const StageSwitchAccesser* stageSwitchAccessers = nullptr;
    s32 accesserSize = 0;
    s32 accesserCapacity = 0;
};

class StageSwitchKeeper {
public:
    StageSwitchKeeper();
    void init(StageSwitchDirector* director, const PlacementInfo& placementInfo);
    StageSwitchAccesser* tryGetStageSwitchAccesser(const char* linkName) const;

    void setUseName(IUseName* useName) { mUseName = useName; }

private:
    StageSwitchAccesser* mStageSwitchAccessers = nullptr;
    s32 mAccesserCapacity = 0;
    IUseName* mUseName = nullptr;
};

}  // namespace al
