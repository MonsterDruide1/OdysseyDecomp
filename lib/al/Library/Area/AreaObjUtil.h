#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObj;
class LiveActor;

bool isInAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);
AreaObj* tryFindAreaObj(const IUseAreaObj* area, const char* name, const sead::Vector3f& pos);

bool isInAreaObj(const LiveActor*, const char*);
bool isInDeathArea(const LiveActor*);

AreaObj* tryFindAreaObj(const LiveActor*, const char*);

bool tryGetAreaObjArg(s32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(f32*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);

}  // namespace al
