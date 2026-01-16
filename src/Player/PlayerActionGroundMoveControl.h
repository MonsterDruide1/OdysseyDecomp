#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IUsePlayerHack;
class IJudge;
class PlayerActionTurnControl;

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
    al::LiveActor* mParent;
    const PlayerConst* mPlayerConst;
    const PlayerInput* mPlayerInput;
    const IUsePlayerCollision* mCollision;
    const IUsePlayerHack** mHack = nullptr;
    bool mIsSetup = false;
    sead::Vector3f mGroundNormal;
    f32 mMaxSpeed;
    f32 mMinSpeed;
    f32 _40;
    s32 _44;
    s32 mRunFrame;
    s32 mStickOnBrakeFrame;
    s32 mBrakeFrame;
    f32 mGravityMove;
    f32 mBrakeSpeed;
    s32 mCounterBorder;
    s32 _60;
    bool _64;
    IJudge* mJudge;
    f32 _70;
    s32 _74;
    bool _78;
    f32 _7c;
    bool mHasStopped;
    sead::Vector3f _84;
    f32 _90;
    f32 _94;
    f32 _98;
    bool mIsForceRunCtrlActive;
    f32 _a0;
    bool mAlwaysFalse;
    f32 _a8;
    f32 _ac;
    PlayerActionTurnControl* mTurnCtrl;
    bool _b8;
    bool _b9;
    bool _ba;
    bool _bb;
    bool _bc;
    bool _bd;
    f32 _c0;
    bool _c4;
    sead::Vector3f _c8;
    bool _d4;
    bool _d5;
};

static_assert(sizeof(PlayerActionGroundMoveControl) == 0xD8);
