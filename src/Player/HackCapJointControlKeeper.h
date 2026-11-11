#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

// Joint-angle keeper for Cappy (16 bytes: three f32 rotation angles plus a
// fourth word). The ctor zeroes all 16 bytes; initCapJointControl wires the
// angles up as X/Y/Z joint-local rotators on the actor. Layout reconstructed
// from HackCapJointControlKeeper.o (ctor at 0x40ad34, initCapJointControl at
// 0x40ad3c); member names are placeholders until the real ones are known.
class HackCapJointControlKeeper {
public:
    HackCapJointControlKeeper();

    void initCapJointControl(al::LiveActor* actor);
    void initDisplayCapJointControl(al::LiveActor* actor);
    void updateRotateThrowZ(f32, f32);
    void updateRotateStayZ(f32);
    void updateRotateY(f32);

    f32 mRotateX = 0.0f;
    f32 mRotateY = 0.0f;
    f32 mRotateZ = 0.0f;
    f32 _c = 0.0f;
};
