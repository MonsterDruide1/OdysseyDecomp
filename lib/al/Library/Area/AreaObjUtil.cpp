#include "Library/Area/AreaObjUtil.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/IUseAreaObj.h"

namespace al {
AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& position) {
    return area->getAreaObjDirector()->getInVolumeAreaObj(name, position);
}

AreaObj* tryFindAreaObjWithFilter(const IUseAreaObj* area, const char* name,
                                  const sead::Vector3f& position, AreaObjFilterBase* filter) {
    AreaObjGroup* areaObjGroup = area->getAreaObjDirector()->getAreaObjGroup(name);
    if (areaObjGroup == nullptr)
        return nullptr;

    AreaObj* currentAreaObj = nullptr;
    s32 size = areaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = areaObjGroup->getAreaObj(i);

        if ((currentAreaObj == nullptr ||
             currentAreaObj->getPriority() <= areaObj->getPriority()) &&
            areaObj->isInVolume(position) && filter->isValid(areaObj)) {
            currentAreaObj = areaObj;
        }
    }

    return currentAreaObj;
}
}  // namespace al
