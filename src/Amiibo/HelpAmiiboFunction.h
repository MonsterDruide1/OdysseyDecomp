#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

namespace HelpAmiiboFunction {
void calcLifeUpItemAppearOffset(sead::Vector3f*, const al::LiveActor*);
void calcCoinAppearQT(sead::Quatf*, sead::Vector3f*, const al::LiveActor*);
bool calcIsHitArrowItemAppearPos(const al::LiveActor*);
}  // namespace HelpAmiiboFunction
