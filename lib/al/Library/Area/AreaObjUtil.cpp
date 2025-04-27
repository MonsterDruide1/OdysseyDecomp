#include "Library/Area/AreaObjUtil.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaShape.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

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
    return tryGetAreaObj(group, position) != nullptr;
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
    return tryFindAreaObj(areaUser, name, position) != nullptr;
}

bool isExistAreaObj(const IUseAreaObj* areaUser, const char* name) {
    return areaUser->getAreaObjDirector()->isExistAreaGroup(name);
}

bool isInDeathArea(const IUseAreaObj* areaUser, const sead::Vector3f& position) {
    return isInAreaObj(areaUser, "DeathArea", position);
}

bool isInWaterArea(const IUseAreaObj* areaUser, const sead::Vector3f& position) {
    return isInAreaObj(areaUser, "WaterArea", position);
}

bool isInPlayerControlOffArea(const IUseAreaObj* areaUser, const sead::Vector3f& position) {
    return isInAreaObj(areaUser, "PlayerControlOffArea", position);
}

s32 calcAreaObjNum(const IUseAreaObj* areaUser, const char* name) {
    AreaObjGroup* group = tryFindAreaObjGroup(areaUser, name);
    if (group == nullptr)
        return 0;

    return group->getSize();
}

f32 calcWaterSinkDepth(const IUseAreaObj* areaUser, const sead::Vector3f& position) {
    AreaObj* areaObj = tryFindAreaObj(areaUser, "WaterArea", position);
    if (!areaObj)
        return -1.0f;

    sead::Vector3f hitPosition;
    sead::Vector3f normal;
    sead::Vector3f position2 = {position.x, position.y + 100000.0f, position.z};

    if (!areaObj->getAreaShape()->checkArrowCollision(&hitPosition, &normal, position, position2))
        return -1.0f;

    return hitPosition.y - position.y;
}

bool tryGetAreaObjArg(s32* outArg, const AreaObj* areaObj, const char* key) {
    if (areaObj->getPlacementInfo() == nullptr)
        return false;

    return tryGetArg(outArg, *areaObj->getPlacementInfo(), key);
}

bool tryGetAreaObjArg(f32* outArg, const AreaObj* areaObj, const char* key) {
    if (areaObj->getPlacementInfo() == nullptr)
        return false;

    return tryGetArg(outArg, *areaObj->getPlacementInfo(), key);
}

bool tryGetAreaObjArg(bool* outArg, const AreaObj* areaObj, const char* key) {
    if (areaObj->getPlacementInfo() == nullptr)
        return false;

    return tryGetArg(outArg, *areaObj->getPlacementInfo(), key);
}

bool tryGetAreaObjStringArg(const char** outArg, const AreaObj* areaObj, const char* key) {
    if (areaObj->getPlacementInfo() == nullptr)
        return false;

    return tryGetStringArg(outArg, *areaObj->getPlacementInfo(), key);
}

const sead::Matrix34f& getAreaObjBaseMtx(const AreaObj* areaObj) {
    return areaObj->getAreaMtx();
}

const sead::Vector3f& getAreaObjScale(const AreaObj* areaObj) {
    return areaObj->getAreaShape()->getScale();
}

void getAreaObjDirFront(sead::Vector3f* outFrontDir, const AreaObj* areaObj) {
    getAreaObjBaseMtx(areaObj).getBase(*outFrontDir, 2);
}

void getAreaObjDirUp(sead::Vector3f* outUpDir, const AreaObj* areaObj) {
    getAreaObjBaseMtx(areaObj).getBase(*outUpDir, 1);
}

void getAreaObjDirSide(sead::Vector3f* outSideDir, const AreaObj* areaObj) {
    getAreaObjBaseMtx(areaObj).getBase(*outSideDir, 0);
}

void calcNearestAreaObjEdgePos(sead::Vector3f* outNearestEdgePos, const AreaObj* areaObj,
                               const sead::Vector3f& position) {
    areaObj->getAreaShape()->calcNearestEdgePoint(outNearestEdgePos, position);
}

void calcNearestAreaObjEdgePosTopY(sead::Vector3f* outNearestEdgePosTopY, const AreaObj* areaObj,
                                   const sead::Vector3f& position) {
    sead::Vector3f scale = {0.0f, 0.0f, 0.0f};
    tryGetScale(&scale, *areaObj->getPlacementInfo());

    sead::Vector3f pos = getAreaObjBaseMtx(areaObj).getBase(3);

    sead::Vector3f sideDir;
    getAreaObjDirSide(&sideDir, areaObj);

    sead::Vector3f upDir;
    getAreaObjDirUp(&upDir, areaObj);

    sead::Vector3f vertical;
    vertical.x = position.x - pos.x;
    vertical.y = position.y - pos.y;
    vertical.z = position.z - pos.z;

    verticalizeVec(&vertical, upDir, vertical);
    limitVectorParallelVertical(&vertical, sideDir, scale.x * 1000.0f, scale.z * 1000.0f);

    vertical.setScaleAdd(scale.y * 1000.0f, upDir, vertical);
    outNearestEdgePosTopY->set(vertical + pos);
}
}  // namespace al
