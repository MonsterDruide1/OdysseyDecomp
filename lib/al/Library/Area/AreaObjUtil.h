#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class AreaObjGroup;
class IUseAreaObj;
class AreaObjGroup;

AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& pos);
bool isInAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);

bool isInDeathArea(const IUseAreaObj*, const sead::Vector3f&);

bool tryGetAreaObjArg(s32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(f32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);
AreaObjGroup* tryFindAreaObjGroup(const IUseAreaObj* areaObj, const char* name);
AreaObj* tryGetAreaObj(const AreaObjGroup*, const sead::Vector3f&);

const sead::Matrix34f& getAreaObjBaseMtx(const AreaObj*);
}  // namespace al
