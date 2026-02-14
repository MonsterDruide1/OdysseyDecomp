#pragma once

#include <controller/seadControllerAddon.h>
#include <prim/seadBitFlag.h>

namespace al {

class PadUiKeyInputAddon : public sead::ControllerAddon {
    SEAD_RTTI_OVERRIDE(PadUiKeyInputAddon, sead::ControllerAddon)

public:
    PadUiKeyInputAddon(sead::Controller* controller);

    bool calc() override;

private:
    sead::BitFlag32 mPadTrig = 0;
    sead::BitFlag32 mPadHold = 0;
    sead::BitFlag32 mPadHoldAndPrev = 0;
    sead::BitFlag32 mPadRepeat = 0;
};

static_assert(sizeof(PadUiKeyInputAddon) == 0x38);

}  // namespace al
