#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;

class PlayerActionAirMoveControl {
public:
    PlayerActionAirMoveControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                               const IUsePlayerCollision*, bool);
    void setup(f32, f32, s32, f32, f32, s32, f32);
    void setupTurn(f32, f32, f32, f32, s32, s32, s32);
    void setExtendFrame(s32);
    void setupCollideWallScaleVelocity(f32, f32, f32);
    void verticalizeStartMoveDir(const sead::Vector3f&);
    void update();
    bool isHoldJumpExtend() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&) const;

    void set_40(bool value) { _40 = value; }

    void set_42(bool value) { _42 = value; }

    void set_44(s32 value) { _44 = value; }

    void set_48(f32 value) { _48 = value; }

    f32 get_64() const { return _64; }

private:
    u8 _0[0x40];
    bool _40;
    u8 _41;
    bool _42;
    u8 _43;
    s32 _44;
    f32 _48;
    u8 _4c[0x64 - 0x4c];
    f32 _64;
    u8 _68[0x90 - 0x68];
};

static_assert(sizeof(PlayerActionAirMoveControl) == 0x90);
