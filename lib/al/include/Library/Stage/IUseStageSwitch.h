#pragma once

#include "Library/HostIO/IUseName.h"

namespace al {
class StageSwitchKeeper;

class IUseStageSwitch : virtual public IUseName {
public:
    virtual StageSwitchKeeper* getStageSwitchKeeper() const = 0;
    virtual void initStageSwitchKeeper() = 0;
};

}  // namespace al
