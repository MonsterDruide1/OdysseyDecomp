#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObj;
class IUseAreaObj;

AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
bool isInAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);

bool tryGetAreaObjArg(s32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(f32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);

}  // namespace al
