#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
class IUseAreaObj;
}  // namespace al

namespace rs {
bool isInChangeStageArea(const al::LiveActor* actor, const sead::Vector3f* pos);
bool tryFindForceRecoveryArea(sead::Vector3f* targetPos, sead::Vector3f* targetUp,
                              const al::AreaObj** area, const al::IUseAreaObj* obj,
                              const sead::Vector3f& pos);
}  // namespace rs
