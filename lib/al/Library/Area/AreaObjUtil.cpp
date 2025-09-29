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
            areaObj->isInVolume(position) && filter->isValidArea(areaObj)) {
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

    bool foundAnyArea = false;
    s32 size = areaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = areaObjGroup->getAreaObj(i);

        if (areaObj->isInVolume(position)) {
            callBack->findArea(areaObj);
            foundAnyArea = true;
        }
    }

    return foundAnyArea;
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

    if (!checkAreaObjCollisionByArrow(&hitPosition, &normal, areaObj, position, position2))
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

bool calcNearestAreaObjEdgePos(sead::Vector3f* outNearestEdgePos, const AreaObj* areaObj,
                               const sead::Vector3f& position) {
    return areaObj->getAreaShape()->calcNearestEdgePoint(outNearestEdgePos, position);
}

void calcNearestAreaObjEdgePosTopY(sead::Vector3f* outNearestEdgePosTopY, const AreaObj* areaObj,
                                   const sead::Vector3f& position) {
    sead::Vector3f scale = {0.0f, 0.0f, 0.0f};
    tryGetScale(&scale, *areaObj->getPlacementInfo());

    sead::Vector3f areaPos = getAreaObjBaseMtx(areaObj).getBase(3);

    sead::Vector3f sideDir;
    getAreaObjDirSide(&sideDir, areaObj);

    sead::Vector3f upDir;
    getAreaObjDirUp(&upDir, areaObj);

    sead::Vector3f diff;
    diff.x = position.x - areaPos.x;
    diff.y = position.y - areaPos.y;
    diff.z = position.z - areaPos.z;

    verticalizeVec(&diff, upDir, diff);
    limitVectorParallelVertical(&diff, sideDir, scale.x * 1000.0f, scale.z * 1000.0f);

    diff.setScaleAdd(scale.y * 1000.0f, upDir, diff);
    outNearestEdgePosTopY->set(diff + areaPos);
}

f32 calcNearestAreaObjEdgeRateTopY(const AreaObj* areaObj, const sead::Vector3f& position) {
    sead::Vector3f areaPos = getAreaObjBaseMtx(areaObj).getBase(3);

    sead::Vector3f upDir;
    getAreaObjDirUp(&upDir, areaObj);

    sead::Vector3f nearestEdgePosTopY;
    calcNearestAreaObjEdgePosTopY(&nearestEdgePosTopY, areaObj, position);

    f32 distToAreaPos = upDir.dot(nearestEdgePosTopY - areaPos);
    if (distToAreaPos < 0.0f || isNearZero(distToAreaPos))
        return 1.0f;

    f32 distToPosition = sead::Mathf::clampMin(upDir.dot(nearestEdgePosTopY - position), 0.0f);

    return sead::Mathf::clamp(distToPosition / distToAreaPos, 0.0f, 1.0f);
}

void calcAreaObjCenterPos(sead::Vector3f* outCenterPosition, const AreaObj* areaObj) {
    sead::Vector3f pos = getAreaObjBaseMtx(areaObj).getBase(3);
    f32 scaleY = getAreaObjScale(areaObj).y;

    outCenterPosition->x = pos.x;
    outCenterPosition->y = pos.y + scaleY * 1000.0f / 2.0f;
    outCenterPosition->z = pos.z;
}

bool checkAreaObjCollisionByArrow(sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                                  const AreaObj* areaObj, const sead::Vector3f& position1,
                                  const sead::Vector3f& position2) {
    return areaObj->getAreaShape()->checkArrowCollision(outHitPosition, outNormal, position1,
                                                        position2);
}

bool calcFindAreaSurface(const IUseAreaObj* areaUser, const char* name,
                         sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                         const sead::Vector3f& position1, const sead::Vector3f& position2) {
    if (tryFindAreaObj(areaUser, name, position1))
        return false;

    sead::Vector3f positionHalfway = (position1 + position2) / 2.0f;

    AreaObj* areaObj = tryFindAreaObj(areaUser, name, positionHalfway);
    if (!areaObj) {
        areaObj = tryFindAreaObj(areaUser, name, position2);
        if (!areaObj)
            return false;
    }

    return checkAreaObjCollisionByArrow(outHitPosition, outNormal, areaObj, position1, position2);
}

bool calcFindAreaSurface(const IUseAreaObj* areaUser, const char* name,
                         sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                         const sead::Vector3f& position, const sead::Vector3f& direction,
                         f32 distance) {
    sead::Vector3f position1;
    sead::Vector3f position2;
    position1.setScaleAdd(distance, direction, position);
    position2.setScaleAdd(distance, -direction, position);

    if (tryFindAreaObj(areaUser, name, position1))
        return false;

    AreaObj* areaObj = tryFindAreaObj(areaUser, name, position);
    if (!areaObj) {
        areaObj = tryFindAreaObj(areaUser, name, position2);
        if (!areaObj)
            return false;
    }

    return checkAreaObjCollisionByArrow(outHitPosition, outNormal, areaObj, position1, position2);
}
}  // namespace al
