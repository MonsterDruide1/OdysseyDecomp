#pragma once

#include <basis/seadTypes.h>
#include <controller/seadAccelerometerAddon.h>

namespace al {
class JoyPadAccelerometerAddon : public sead::AccelerometerAddon {
public:
    JoyPadAccelerometerAddon(sead::Controller* controller, s32 index);

    bool calc() override;

private:
    s32 mIndex;
};

static_assert(sizeof(JoyPadAccelerometerAddon) == 0x40);
}  // namespace al
