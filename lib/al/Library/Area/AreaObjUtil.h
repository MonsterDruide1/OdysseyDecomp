#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class AreaObjFilterBase;
class AreaObjFindCallBack;
class AreaObjGroup;
class IUseAreaObj;

class AreaObjFilterBase {
public:
    virtual bool isValidArea(AreaObj* areaObj) const = 0;
};

class AreaObjFindCallBack {
public:
    virtual void findArea(const AreaObj* areaObj) = 0;
};

AreaObj* tryFindAreaObj(const IUseAreaObj* areaUser, const char* name,
                        const sead::Vector3f& position);
AreaObj* tryFindAreaObjWithFilter(const IUseAreaObj* areaUser, const char* name,
                                  const sead::Vector3f& position, AreaObjFilterBase* filter);
bool tryFindAreaObjAll(const IUseAreaObj* areaUser, const char* name,
                       const sead::Vector3f& position, AreaObjFindCallBack* callBack);
AreaObjGroup* tryFindAreaObjGroup(const IUseAreaObj* areaUser, const char* name);
bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& position);
AreaObj* tryGetAreaObj(const AreaObjGroup* group, const sead::Vector3f& position);
bool isInAreaPos(const AreaObj* areaObj, const sead::Vector3f& position);
bool isInAreaObj(const IUseAreaObj* areaUser, const char* name, const sead::Vector3f& position);
bool isExistAreaObj(const IUseAreaObj* areaUser, const char* name);
bool isInDeathArea(const IUseAreaObj* areaUser, const sead::Vector3f& position);
bool isInWaterArea(const IUseAreaObj* areaUser, const sead::Vector3f& position);
bool isInPlayerControlOffArea(const IUseAreaObj* areaUser, const sead::Vector3f& position);
s32 calcAreaObjNum(const IUseAreaObj* areaUser, const char* name);
f32 calcWaterSinkDepth(const IUseAreaObj* areaUser, const sead::Vector3f& position);
bool tryGetAreaObjArg(s32* outArg, const AreaObj* areaObj, const char* key);
bool tryGetAreaObjArg(f32* outArg, const AreaObj* areaObj, const char* key);
bool tryGetAreaObjArg(bool* outArg, const AreaObj* areaObj, const char* key);
bool tryGetAreaObjStringArg(const char** outArg, const AreaObj* areaObj, const char* key);
const sead::Matrix34f& getAreaObjBaseMtx(const AreaObj* areaObj);
const sead::Vector3f& getAreaObjScale(const AreaObj* areaObj);
void getAreaObjDirFront(sead::Vector3f* outFrontDir, const AreaObj* areaObj);
void getAreaObjDirUp(sead::Vector3f* outUpDir, const AreaObj* areaObj);
void getAreaObjDirSide(sead::Vector3f* outSideDir, const AreaObj* areaObj);
void calcNearestAreaObjEdgePos(sead::Vector3f* outNearestEdgePos, const AreaObj* areaObj,
                               const sead::Vector3f& position);
void calcNearestAreaObjEdgePosTopY(sead::Vector3f* outNearestEdgePosTopY, const AreaObj* areaObj,
                                   const sead::Vector3f& position);
f32 calcNearestAreaObjEdgeRateTopY(const AreaObj* areaObj, const sead::Vector3f& position);
void calcAreaObjCenterPos(sead::Vector3f* outCenterPosition, const AreaObj* areaObj);
bool checkAreaObjCollisionByArrow(sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                                  const AreaObj* areaObj, const sead::Vector3f& position1,
                                  const sead::Vector3f& position2);
bool calcFindAreaSurface(const IUseAreaObj* areaUser, const char* name,
                         sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                         const sead::Vector3f& position1, const sead::Vector3f& position2);
bool calcFindAreaSurface(const IUseAreaObj* areaUser, const char* name,
                         sead::Vector3f* outHitPosition, sead::Vector3f* outNormal,
                         const sead::Vector3f& position, const sead::Vector3f& direction,
                         f32 distance);
}  // namespace al
