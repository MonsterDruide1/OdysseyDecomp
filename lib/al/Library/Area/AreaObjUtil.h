#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObj;
class AreaObjGroup;
class IUseAreaObj;

AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
bool isInAreaObj(const AreaObjGroup* group, const sead::Vector3f& pos);
bool isInAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);

bool isInDeathArea(const IUseAreaObj*, const sead::Vector3f&);

bool tryGetAreaObjArg(s32* out, const AreaObj* area, const char* name);
bool tryGetAreaObjArg(f32* out, const AreaObj* area, const char* name);
bool tryGetAreaObjArg(bool* out, const AreaObj* area, const char* name);
bool tryGetAreaObjStringArg(const char** out, const AreaObj* area, const char* name);

}  // namespace al
