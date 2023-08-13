#pragma once

namespace al {
class AreaObj;
class LiveActor;

bool isInAreaObj(const LiveActor*, const char*);
bool isInDeathArea(const LiveActor*);

AreaObj* tryFindAreaObj(const LiveActor*, const char*);

bool tryGetAreaObjArg(int*, const AreaObj*, const char*);
bool tryGetAreaObjArg(float*, const AreaObj*, const char*);
bool tryGetAreaObjArg(bool*, const AreaObj*, const char*);
bool tryGetAreaObjStringArg(const char**, const AreaObj*, const char*);

}  // namespace al
