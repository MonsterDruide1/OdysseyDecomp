#pragma once

#include "math/seadVector.h"
#include "prim/seadSafeString.h"

namespace al {
class HitSensor;
}
class HackCap;
class PlayerConst;
class PlayerTrigger;
class PlayerInput;
class PlayerCounterAfterCapCatch;
class PlayerJudgePreInputCapThrow;
class PlayerAnimator;

class PlayerSpinCapAttack {
public:
    PlayerSpinCapAttack(HackCap*, PlayerConst const*, PlayerTrigger const*, PlayerInput const*,
                        PlayerCounterAfterCapCatch const*, PlayerJudgePreInputCapThrow const*);
    void clearAttackInfo();
    void setupAttackInfo();
    void startCapSpinAttack(PlayerAnimator*, PlayerInput const*);
    void startCapSpinAttackAir(PlayerAnimator*, PlayerInput const*);
    void startCapSpinAttackSwim(PlayerAnimator*, PlayerInput const*);
    void startSpinSeparate(PlayerAnimator*);
    void startSpinSeparateSwim(PlayerAnimator*);
    void startSpinSeparateSwimSurface(PlayerAnimator*);
    void startCapThrow(sead::Vector3<float> const&, sead::Vector3<float> const&, float, bool,
                       sead::Vector3<float> const&);
    void attackSpinMsg(al::HitSensor*, al::HitSensor*);
    bool tryCancelCapState(PlayerAnimator*);
    bool tryStartCapSpinGroundMiss(PlayerAnimator*);
    bool tryStartCapSpinAirMiss(PlayerAnimator*);
    bool isCapSpinAttack() const;
    bool isValidAttackSensor(PlayerAnimator const*) const;
    bool isEnablePlaySpinCapMiss(PlayerAnimator const*) const;
    bool isSeparateSingleSpin() const;
    bool isThrowSwingRightDir() const;
    s32 getThrowFrameGround() const;
    s32 getThrowFrameAir() const;
    s32 getThrowFrameSwim() const;

private:
    HackCap* mHackCap;
    const PlayerConst* mConst;
    const PlayerTrigger* mTrigger;
    const PlayerInput* mInput;
    const PlayerCounterAfterCapCatch* mCounterAfterCapCatch;
    const PlayerJudgePreInputCapThrow* mJudgePreInputCapThrow;
    bool _30 = 0;
    bool _31 = 0;
    sead::Vector3f _34 = {0.0f, 0.0f, 0.0f};
    int _40 = 0;
    bool _44 = 0;
    bool _45 = 0;
    sead::Vector2f _48 = {0.0f, 0.0f};
    sead::Vector2f _50 = {0.0f, 0.0f};
    float _58 = 0;
    sead::FixedSafeString<64> _60 = {""};
    sead::FixedSafeString<64> _B8 = {""};
};
