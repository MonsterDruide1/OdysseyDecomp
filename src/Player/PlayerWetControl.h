#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al
class PlayerAreaChecker;
class PlayerEffect;

// Minimal header: only the declarations needed by HackCap::movement.
// The class itself is NotDecompiled (Player/PlayerWetControl.o).
// Layout from PlayerWetControl.o (ctor 0x492e34, 40 B): +0x00 the actor,
// +0x08 nullptr, +0x10 a second LiveActor*, +0x18 the PlayerAreaChecker*,
// +0x20 a bool, +0x24..0x2f a 12-byte block (s32 -1, s32 -1, f32 1.0f),
// +0x30 a bool. sizeof 0x38.
class PlayerWetControl {
public:
    PlayerWetControl(const al::LiveActor*, al::LiveActor*, const PlayerAreaChecker*);

    void update();

    void reset();
    void updateModelRoughness(f32 roughness);

    bool isWet() const;
    void recordInWater();
    void recordWet();
    void recordWaterSurface();
    void recordHeavyLandPuddle();
    void recordPuddleRolling();
    void recordForestWaterFall();
    void recordWaterSplash();
    void recordWetBySensor();

    const al::LiveActor* _0;
    PlayerEffect* _8 = nullptr;
    al::LiveActor* _10;
    const PlayerAreaChecker* _18;
    bool _20 = false;
    char _21[3];
    // 12-byte block at +0x24, copied from mPlayerWetControl to
    // mSelfPlayerWetControl by HackCap::startThrow when the cap is not in
    // the Hack nerve. Packed so the copy loads as the original does
    // (w + x, split for the store). Layout is a placeholder until the
    // real members are known.
    struct __attribute__((packed)) {
        s32 _24 = -1;
        s32 _28 = -1;
        f32 _2c = 1.0f;
    } _24;
    bool _30 = false;
    char _31[7];
};
