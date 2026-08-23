#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class GamePadSystem;
}

struct PlayerInitInfo {
    al::GamePadSystem* gamePadSystem = nullptr;
    sead::Matrix34f* viewMtx = nullptr;
    u32 portNo = 0;
    const char* modelName = nullptr;
    const char* capTypeName = nullptr;
    sead::Vector3f trans = sead::Vector3f::zero;
    sead::Quatf quat = sead::Quatf::unit;
    bool isNeedCreateNoseNeedle = false;
    bool isClosetScenePlayer = false;
};

static_assert(sizeof(PlayerInitInfo) == 0x48);
