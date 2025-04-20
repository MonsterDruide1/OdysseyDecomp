#include "Library/Area/AreaObjUtil.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/IUseAreaObj.h"

namespace al {
AreaObj* tryFindAreaObj(const IUseAreaObj* areaUser, const char* name,
                        const sead::Vector3f& position) {
    return areaUser->getAreaObjDirector()->getInVolumeAreaObj(name, position);
}

AreaObj* tryFindAreaObjWithFilter(const IUseAreaObj* areaUser, const char* name,
                                  const sead::Vector3f& position, AreaObjFilterBase* filter) {
    AreaObjGroup* areaObjGroup = tryFindAreaObjGroup(areaUser, name);
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

bool tryFindAreaObjAll(const IUseAreaObj* areaUser, const char* name,
                       const sead::Vector3f& position, AreaObjFindCallBack* callBack) {
    AreaObjGroup* areaObjGroup = tryFindAreaObjGroup(areaUser, name);
    if (areaObjGroup == nullptr)
        return false;

    bool isValid = false;
    s32 size = areaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = areaObjGroup->getAreaObj(i);

        if (areaObj->isInVolume(position)) {
            callBack->findArea(areaObj);
            isValid = true;
        }
    }

    return isValid;
}

AreaObjGroup* tryFindAreaObjGroup(const IUseAreaObj* areaUser, const char* name) {
    return areaUser->getAreaObjDirector()->getAreaObjGroup(name);
}

bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& position) {
    if (group == nullptr)
        return false;

    return group->getInVolumeAreaObj(position) != nullptr;
}

AreaObj* tryGetAreaObj(const AreaObjGroup* group, const sead::Vector3f& position) {
    if (group == nullptr)
        return nullptr;

    return group->getInVolumeAreaObj(position);
}

bool isInAreaPos(const AreaObj* areaObj, const sead::Vector3f& position) {
    return areaObj->isInVolume(position);
}

bool isInAreaObj(const IUseAreaObj* areaUser, const char* name, const sead::Vector3f& position) {
    return areaUser->getAreaObjDirector()->getInVolumeAreaObj(name, position) != nullptr;
}
}  // namespace al
