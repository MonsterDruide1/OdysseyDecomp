#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "PlayerUtil.h"

namespace al {
class MtxConnector;
}

class PlayerStateWait : public al::ActorStateBase {
public:
    PlayerStateWait(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerModelChangerHakoniwa const*,PlayerJointControlKeeper const*,al::WaterSurfaceFinder const*,IUsePlayerHeightCheck const*,IJudge const*,PlayerAnimator *,PlayerTrigger *,PlayerCapManHeroEyesControl *,PlayerJointParamCenterDynamics *,PlayerJointParamGroundPose *);
    ~PlayerStateWait() override;
    
    void appear() override;
    void kill() override;
    void control() override;

    bool isWait() const;
    bool isEnableLookAt() const;
    bool isEnableCancelAction() const;
    bool isEnableCancelHipDropJump() const;
    bool isEnableRecoveryLife() const;
    bool isEnableReactionCapCatch() const;
    bool isAreaAnimCapManHeroEyes() const;
    bool isPlaySwitchOnAreaAnim() const;
    bool isLandStain() const;
    bool isSleep() const;
    bool attackHipDropKnockDown(al::HitSensor *,al::HitSensor *);
    bool tryClearIgnoreSwitchOnAreaAnim();
    bool requestAnimName(char const*);
    void noticeCarryStart();
    void initSceneStartAnim();
    bool tryConnectWait();
    f32 getInverseKinematicsRate() const;
    bool tryUpdateAreaAnim();
    bool tryChangeRequestAnim();
    bool tryChangeAreaAnim();
    bool tryGetSpecialStatusAnimName(char const**);
    
    void exeLandStiffen();
    void exeLand();
    void exeWait();
    void exeWaitConnect2D();
    void exeWaitRelaxStart();
    void exeWaitRelax();
    void exeWaitSnoozeStart();
    void exeWaitSnooze();
    void exeWaitSleepStart();
    void exeWaitSleep();
    void exeAreaAnimTurn();
    void exeAreaAnimWait();
    void exeRequestAnimWait();

private:
    const PlayerConst *mConst;
    const IUsePlayerCollision *mCollider;
    const PlayerModelChangerHakoniwa *mModelChanger;
    const PlayerJointControlKeeper *mJointControlKeeper;
    const al::WaterSurfaceFinder *mWaterSurfaceFinder;
    const IUsePlayerHeightCheck *mHeightCheck;
    const IJudge *mJudgeForceLand;
    PlayerAnimator *mAnimator;
    PlayerTrigger *mTrigger;
    PlayerCapManHeroEyesControl *mCapEyesControl;
    PlayerJointParamCenterDynamics *mJointParamCenterDynamics;
    PlayerJointParamGroundPose *mJointParamGroundPose;
    bool mIsIgnoreSwitchOnAreaAnim = 0;
    al::AreaObj *mAnimArea = 0;
    const char *_90 = 0;
    sead::Vector3f _98 = {0.0f, 0.0f, 0.0f};
    int _A4 = 0;
    int _A8 = 0;
    bool _AC = 0;
    bool _AD = 0;
    bool _AE = 0;
    bool pad_AF = 0;
    int _B0 = 0;
    bool _B4 = 0;
    const char *mRequestAnimName = 0;
    sead::Vector3f _C0 = {0.0f, 0.0f, 0.0f};
    int _CC = 0;
    al::MtxConnector *_D0 = 0;
    sead::Vector3f _D8 = {0.0f, 0.0f, 0.0f};
    bool _E4 = 0;
};
