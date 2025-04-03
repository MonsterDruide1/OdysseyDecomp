#include "Area/StainArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Placement/PlacementFunction.h"

StainArea::StainArea(const char* name) : al::AreaObj(name) {}

void StainArea::init(const al::AreaInitInfo& info) {
    AreaObj::init(info);
    s32 stainType = 0;
    al::tryGetArg(&stainType, info, "StainType");
    mStainType = (StainType)stainType;
}
