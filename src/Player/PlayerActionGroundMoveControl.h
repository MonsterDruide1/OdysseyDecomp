#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IJudge;
class IUsePlayerCollision;

class PlayerActionGroundMoveControl {
public:
    PlayerActionGroundMoveControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                                  const IUsePlayerCollision*);

    void initDash(IJudge*, f32, s32);
    void setupDash(f32, s32);
    void setup(f32, f32, s32, s32, s32, f32, f32, s32);
    void appear();
    void reset(const sead::Vector3f&);
    void calcInitBrakeOnCounter();
    f32 update();
    f32 updateSkateMove();
    f32 updateNormalMove();
    void updateNormalAndSnap(sead::Vector3f*);
    f32 calcTurnTiltRate() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&);
    bool isActiveSquatBrake() const;
    void updateHillAffect(const sead::Vector3f&, const sead::Vector3f&, bool);
    f32 calcMaxSpeed(f32) const;
    f32 calcAccelRate(f32) const;
    void updatePoseUpFront(const sead::Vector3f&, const sead::Vector3f&, f32);

    const sead::Vector3f& getGroundNormal() const { return mGroundNormal; }
    
    void set_c4(bool c4) { _c4 = c4; }

private:
    void* _0[5];
    bool _28;
    sead::Vector3f mGroundNormal;
    void* _38[17];
    f32 _c0;
    bool _c4;
    void* _c8[2];
};

static_assert(sizeof(PlayerActionGroundMoveControl) == 0xD8);
