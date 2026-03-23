#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

class HosuiTrail;

namespace al {
struct ActorInitInfo;
}  // namespace al

class HosuiTrailKeeper : public al::ISceneObj, public al::HioNode {
public:
    HosuiTrailKeeper(const al::ActorInitInfo& initInfo);

    void appearTrail(const sead::Vector3f& pos, const sead::Vector3f& dir);
    void forceKillAll();

private:
    sead::PtrArray<HosuiTrail> mTrails;
    s32 mIndex = 0;
};

static_assert(sizeof(HosuiTrailKeeper) == 0x20);
