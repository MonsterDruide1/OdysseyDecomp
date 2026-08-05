#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class EquipmentInfo {
public:
    EquipmentInfo(const al::LiveActor*);

    void onNoCapThrow();
    void onForceDash(s32, f32);
    bool isNoCapThrow() const;
    bool isForceDash() const;

    s32 getForceDashFrame() const { return mForceDashFrame; }

    f32 getForceDashSpeed() const { return mForceDashSpeed; }

private:
    const al::LiveActor* mActor = nullptr;
    u32 mFlags = 0;
    s32 mForceDashFrame = 0;
    f32 mForceDashSpeed = 0.0f;
};

static_assert(sizeof(EquipmentInfo) == 0x18);
