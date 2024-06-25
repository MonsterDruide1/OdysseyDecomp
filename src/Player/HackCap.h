#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "PlayerUtil.h"

class HackCap : public al::LiveActor {
public:
    enum class SwingHandType {};
    
    HackCap(al::LiveActor const*,char const*,PlayerInput const*,PlayerAreaChecker const*,PlayerWallActionHistory const*,PlayerCapActionHistory const*,PlayerEyeSensorHitHolder const*,PlayerSeparateCapFlag const*,IUsePlayerCollision const*,IUsePlayerHeightCheck const*,PlayerWetControl const*,PlayerJointControlKeeper const*,HackCapJudgePreInputSeparateThrow *,HackCapJudgePreInputSeparateJump *);
    void calcReturnTargetPos(sead::Vector3f*);

    void hide(bool);
    void forcePutOn();
    bool isNoPutOnHide();

    bool isFlying() const;

    bool isThrowTypeSpiral() const;
    bool isEnableSpinAttack();
    bool isRequestableReturn() const;
    bool isEnablePreInput() const;
    bool isEnableThrowSeparate() const;
    bool requestReturn(bool*);
    void updateSeparateMode(const PlayerSeparateCapFlag*);

    bool sendMsgStartHack(al::HitSensor *);

    void startCatch(char const*,bool,sead::Vector3<float> const&);
    void prepareCooperateThrow();

    void startThrowSeparatePlay(sead::Vector3<float> const&,sead::Vector3<float> const&,float,bool);
    void startThrowSeparatePlayJump(sead::Vector3<float> const&,sead::Vector3<float> const&,float);

    f32 calcSeparateHideSpeedH(sead::Vector3<float> const&);

    void startSpinAttack(const sead::SafeString&);

    bool cancelCapState();
    bool isSpinAttack();

    void startThrow(bool,sead::Vector3<float> const&,sead::Vector3<float> const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector3<float> const&,bool,sead::Vector3<float> const&,HackCap::SwingHandType,bool,float,int);
    void attackSpin(al::HitSensor *,al::HitSensor *,float);

    unsigned char padding_to_118[0x10];
    al::LiveActor* mActorA;
    unsigned char padding_08[0x08];
    al::LiveActor* mPlayerActor;
};
