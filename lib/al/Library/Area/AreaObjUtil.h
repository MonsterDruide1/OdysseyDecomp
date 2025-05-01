#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class AreaObjFilterBase;
class AreaObjFindCallBack;
class AreaObjGroup;
class IUseAreaObj;

AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
AreaObj* tryFindAreaObjWithFilter(const IUseAreaObj*, const char*, const sead::Vector3f&,
                                  AreaObjFilterBase*);
AreaObj* tryFindAreaObjAll(const IUseAreaObj*, const char*, const sead::Vector3f&,
                           AreaObjFindCallBack*);
AreaObjGroup* tryFindAreaObjGroup(const IUseAreaObj* areaObj, const char* name);
bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& pos);
AreaObj* tryGetAreaObj(const AreaObjGroup*, const sead::Vector3f&);
bool isInAreaPos(const AreaObj*, const sead::Vector3f&);
bool isInAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
bool isExistAreaObj(const IUseAreaObj*, const char*);
bool isInDeathArea(const IUseAreaObj*, const sead::Vector3f&);
bool isInWaterArea(const IUseAreaObj*, const sead::Vector3f&);
bool isInPlayerControlOffArea(const IUseAreaObj*, const sead::Vector3f&);
s32 calcAreaObjNum(const IUseAreaObj*, const char*);
f32 calcWaterSinkDepth(const IUseAreaObj*, const sead::Vector3f&);
bool tryGetAreaObjArg(s32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(f32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);
const sead::Matrix34f& getAreaObjBaseMtx(const AreaObj*);
f32 getAreaObjScale(const AreaObj*);
const sead::Vector3f& getAreaObjDirFront(sead::Vector3f*, const AreaObj*);
const sead::Vector3f& getAreaObjDirUp(sead::Vector3f*, const AreaObj*);
const sead::Vector3f& getAreaObjDirSide(sead::Vector3f*, const AreaObj*);
void calcNearestAreaObjEdgePos(sead::Vector3f*, const AreaObj*, const sead::Vector3f&);
void calcNearestAreaObjEdgePosTopY(sead::Vector3f*, const AreaObj*, const sead::Vector3f&);
f32 calcNearestAreaObjEdgeRateTopY(const AreaObj*, const sead::Vector3f&);
void calcAreaObjCenterPos(sead::Vector3f*, const AreaObj*);
bool checkAreaObjCollisionByArrow(sead::Vector3f*, sead::Vector3f*, const AreaObj*,
                                  const sead::Vector3f&, const sead::Vector3f&);
bool calcFindAreaSurface(const IUseAreaObj*, const char*, sead::Vector3f*, sead::Vector3f*,
                         const sead::Vector3f&, const sead::Vector3f&);
bool calcFindAreaSurface(const IUseAreaObj*, const char*, sead::Vector3f*, sead::Vector3f*,
                         const sead::Vector3f&, const sead::Vector3f&, f32);

}  // namespace al
