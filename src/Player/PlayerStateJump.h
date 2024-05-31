#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class IPlayerModelChanger;
class PlayerInput;
class PlayerJumpMessageRequest;
class IJudge;
class PlayerContinuousJump;
class IUseDimension;
class PlayerCounterForceRun;
class PlayerJointControlKeeper;
class IUsePlayerCollision;
class PlayerTrigger;
class PlayerAnimator;
class PlayerActionDiveInWater;
class HackCap;
class PlayerActionAirMoveControl;

class PlayerStateJump : public al::ActorStateBase {
public:
    PlayerStateJump(al::LiveActor*, const PlayerConst*, const IPlayerModelChanger*,
                    const IUseDimension*, const PlayerInput*, const PlayerJumpMessageRequest*,
                    const IJudge*, const PlayerCounterForceRun*, const PlayerJointControlKeeper*,
                    const IUsePlayerCollision*, PlayerTrigger*, PlayerContinuousJump*,
                    PlayerAnimator*, PlayerActionDiveInWater*, HackCap*, IJudge*, bool);
    ~PlayerStateJump() override;

    void appear() override;

    f32 calcJumpPowerBorderSpeedMin() const;
    f32 calcJumpPowerBorderSpeedMax() const;
    f32 calcJumpPowerMin() const;
    f32 calcJumpPowerMax() const;
    f32 calcJumpGravity() const;
    void tryCountUpContinuousJump(PlayerContinuousJump*);
    bool isJumpCapCatch() const;
    bool isJumpSpinFlower() const;
    bool isJumpSpinGround() const;
    bool isJumpSpinGroundClockwise() const;
    bool isJumpBack() const;
    bool isHovering() const;
    bool isEndJumpDownFallLand() const;
    bool isHoldDownFall() const;
    bool isEnableHipDropStart() const;
    bool isEnableTrampleByHipDropAttack() const;
    bool isEnableReactionCapCatch() const;
    bool isEnableCancelCarryThrow() const;
    bool isFormSquat2D() const;
    void exeJump();
    bool trySubAnimJumpReaction();
    const char* calcJumpAnimName() const;
    void exeJumpSpinFlower();
    void exeJumpSpinFlowerDownFall();
    void updateNerveDownFall(const char*, f32, f32, f32, const al::Nerve*);
    void exeJumpSpinGround();
    void exeJumpSpinGroundDownFall();
    void exeJumpTurn();
    void exeJumpBack();
    void exeHoveringJump2D();
    void exeHovering2D();

private:
    const PlayerConst *mConst;
    const IPlayerModelChanger *mModelChanger;
    const IUseDimension *mDimension;
    const PlayerJumpMessageRequest *mJumpMessageRequest;
    const IJudge *mJudgeCapCatch;
    const PlayerCounterForceRun *mCounterForceRun;
    const PlayerJointControlKeeper *mJointControlKeeper;
    const IUsePlayerCollision *mCollision;
    PlayerAnimator *mAnimator;
    PlayerContinuousJump *mContinuousJump;
    PlayerTrigger *mTrigger;
    PlayerActionDiveInWater *mActionDiveInWater;
    PlayerActionAirMoveControl *mActionAirMoveControl = nullptr;
    HackCap *mHackCap;
    IJudge *mJudgeHackCapHoldHoveringJump;
    bool mIsMoon;
    int mExtendFrame = 0;
    float mJumpPower = 0.0f;
    float mMoveSpeedMax = 0.0f;
    float mJumpGravity = 0.0f;
    int mSpinFlowerJumpStayCounter = 0;
    int mContinuousJumpCount = 0;
    bool _B4 = false;
    bool _B5 = false;
    bool _B6 = false;
    bool _B7 = false;
    bool _B8 = false;
    bool _B9 = false;
    bool _BA = false;
    sead::Vector3f _BC = {0.0f, 0.0f, 0.0f};
    const char *_C8 = nullptr;
    const char *mJumpAnimName = nullptr;
    const PlayerInput *mInput;
    sead::Vector3f _E0 = {0.0f, 0.0f, 0.0f};
    int _EC = 0;
    bool _F0 = false;
    sead::Vector3f vec = {0.0f, 0.0f, 0.0f};
};
