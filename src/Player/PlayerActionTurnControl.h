#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerActionTurnControl {
public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(f32, f32, f32, f32, s32, s32, s32);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, f32);
    void calcTurnPowerRate(const sead::Vector3f&) const;

    const sead::Vector3f& getFaceDirection() { return mFaceDirection; }

    void set88(bool value) { _88 = value; }

    void set89(bool value) { _89 = value; }

private:
    al::LiveActor* mActor;
    f32 _8;
    sead::Vector3f _c;
    sead::Vector3f _18;
    sead::Vector3f _24;
    f32 _30;
    f32* _38;
    s32 _40;
    s32 _44;
    s32 _48;
    s32 _4c;
    sead::Vector3f _50;
    sead::Vector3f mFaceDirection;
    bool _68;
    bool _69;
    bool _6a;
    f32 _6c;
    f32 _70;
    f32 _74;
    f32 _78;
    s32 _7c;
    s32 _80;
    s32 _84;
    bool _88;
    bool _89;
};

static_assert(sizeof(PlayerActionTurnControl) == 0x90);
