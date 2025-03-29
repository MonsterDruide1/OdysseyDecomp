#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class PlayerHolder;
class SceneCameraInfo;

class SwitchAreaTargetInfo {
public:
    SwitchAreaTargetInfo(s32, s32);
    SwitchAreaTargetInfo(sead::Vector3f*, s32);

    void update(const PlayerHolder*, const SceneCameraInfo*);

    const sead::Vector3f* get_0() const { return _0; }

    s32 get_8() const { return _8; }

private:
    sead::Vector3f* _0;
    s32 _8;
    unsigned char padding[0x24];
};

static_assert(sizeof(SwitchAreaTargetInfo) == 0x30);
}  // namespace al
