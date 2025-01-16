#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

class HelpAmiiboFunction {
public:
    static void calcLifeUpItemAppearOffset(sead::Vector3f*, const al::LiveActor*);
    static void calcCoinAppearQT(sead::Quatf*, sead::Vector3f*, const al::LiveActor*);
    static bool calcIsHitArrowItemAppearPos(const al::LiveActor*);
};
