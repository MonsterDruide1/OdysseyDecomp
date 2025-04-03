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

    const StageSwitchAccesser* accessers = nullptr;
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
    StageSwitchAccesser* mAccessers = nullptr;
    s32 mAccesserSize = 0;
    IUseName* mUseName = nullptr;
};

}  // namespace al
