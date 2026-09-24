#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al

class EquipmentInfo {
public:
    EquipmentInfo(const al::LiveActor* actor);

    void onNoCapThrow();
    void onForceDash(s32, f32);
    bool isNoCapThrow() const;
    bool isForceDash() const;

private:
    const al::LiveActor* mActor;
    u32 _8;
    s32 _c;
    f32 _10;
};

static_assert(sizeof(EquipmentInfo) == 0x18);
