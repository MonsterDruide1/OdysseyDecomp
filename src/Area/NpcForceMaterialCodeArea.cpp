#include "Area/NpcForceMaterialCodeArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Placement/PlacementFunction.h"

NpcForceMaterialCodeArea::NpcForceMaterialCodeArea(const char* name) : al::AreaObj(name) {}

void NpcForceMaterialCodeArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);
    al::getStringArg(&mMaterialCodeName, *getPlacementInfo(), "MaterialCodeName");
}
