#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

bool isExistItemKeeper(const LiveActor* actor);
void setAppearItemOffset(const LiveActor* actor, const sead::Vector3f& offset);

void appearItem(const al::LiveActor* actor);
}  // namespace al
