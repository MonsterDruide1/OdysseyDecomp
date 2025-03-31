#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class PlayerHolder;
class SceneCameraInfo;

class SwitchAreaTargetInfo {
public:
    SwitchAreaTargetInfo(s32 _0_10Count, s32 _20Count);
    SwitchAreaTargetInfo(sead::Vector3f* _0_10, s32 _0_10Count);

    void update(const PlayerHolder* playerHolder, const SceneCameraInfo* sceneCameraInfo);

    const sead::Vector3f* get_0() const { return _0; }

    s32 get_8() const { return _8; }

private:
    sead::Vector3f* _0 = nullptr;
    s32 _8 = 0;
    s32 _c = 0;
    sead::Vector3f* _10 = nullptr;
    s32 _18 = 0;
    s32 _1c = 0;
    sead::Vector3f* _20 = nullptr;
    s32 _28 = 0;
    s32 _2c = 0;
};

static_assert(sizeof(SwitchAreaTargetInfo) == 0x30);
}  // namespace al
