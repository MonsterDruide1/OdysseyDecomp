#pragma once

#include <basis/seadTypes.h>

// Minimal partial reconstruction of the cap's post-movement trigger holder.
// Only the enum and the members used by HackCap are declared so far;
// the class itself is still NotDecompiled (Player/HackCapTrigger.o).
// The ctor (0x40df9c) zeroes a single s32 at +0x00.
class HackCapTrigger {
public:
    HackCapTrigger();

    enum class EAfterMovementTrigger {
        // Placeholder name: the enumerator names are unknown, but the value
        // 0 is what HackCap::startThrowSeparatePlayJump passes to set().
        Val0 = 0,
        Val1 = 1,
        Val2 = 2,
    };

    void set(EAfterMovementTrigger trigger);
    bool isOn(EAfterMovementTrigger trigger) const;
    void clearAfterMovemetTrigger();

private:
    s32 _0 = 0;
};
