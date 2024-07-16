#pragma once

namespace al {
class LiveActor;

bool isExistItemKeeper(const LiveActor* actor);
void setAppearItemOffset(const LiveActor* actor, const sead::Vector3f& offset);
}  // namespace al
