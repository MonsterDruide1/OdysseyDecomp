#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

// Minimal reconstruction of the cap's above-ground checker.
// Layout from HackCapAboveGroundChecker.o (ctor 0x40a184, 28 B; update
// 0x40a1a0): +0x00 the actor, +0x08 a bool (set by update), +0x0c and
// +0x18 two Vector3f (hit pos and normal, from getHitPosAndNormalOnArrow),
// +0x24 and +0x28 two f32 (dot products of the hit offset against the up
// direction and gravity, stored by update). sizeof 0x30. Member names are
// placeholders until the real ones are known.
class HackCapAboveGroundChecker {
public:
    HackCapAboveGroundChecker(const al::LiveActor* actor);

    void update(const sead::Vector3f& gravity);

    // Read by HackCap::updateShadowMaskOffset (direct member access in the
    // original), so these are public.
    const al::LiveActor* mActor;
    bool _8 = false;
    sead::Vector3f _c = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _18 = {0.0f, 0.0f, 0.0f};
    f32 _24 = 0.0f;
    f32 _28 = 0.0f;
    char _2c[4];
};
