#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

bool isActivePrePassLight(const LiveActor*, const char*);
bool isExistPrePassLight(const LiveActor*, const char*);
void killPrePassLight(const LiveActor*, const char*, s32);
void appearPrePassLight(const LiveActor*, const char*, s32);
}  // namespace al
