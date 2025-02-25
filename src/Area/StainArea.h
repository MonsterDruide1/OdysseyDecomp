#pragma once

#include "Library/Area/AreaObj.h"

class StainArea : public al::AreaObj {
public:
    enum class StainType : s32 {
        None = 0,
        Blizzard = 1,
        WaterFall = 2,
        WaterSplash = 3,
        BlackSmoke = 4,
    };

    StainArea(const char* name);
    void init(const al::AreaInitInfo& info) override;

    StainType getStainType() const { return mStainType; }

private:
    StainType mStainType = StainType::None;
};
