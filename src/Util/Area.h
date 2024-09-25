#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
class IUseAreaObj;
}  // namespace al

namespace rs {
bool isInChangeStageArea(const al::LiveActor*, const sead::Vector3f*);
bool tryFindForceRecoveryArea(sead::Vector3f*, sead::Vector3f*, const al::AreaObj**,
                              const al::IUseAreaObj*, const sead::Vector3f&);
}  // namespace rs
