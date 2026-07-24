#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Nerve;
}  // namespace al

class HackCap;
class IJudge;
class IPlayerModelChanger;
class IUseDimension;
class IUsePlayerCollision;
class PlayerActionAirMoveControl;
class PlayerActionDiveInWater;
class PlayerAnimator;
class PlayerConst;
class PlayerContinuousJump;
class PlayerCounterForceRun;
class PlayerInput;
class PlayerJointControlKeeper;
class PlayerTrigger;
struct PlayerJumpMessageRequest;

class PlayerStateJump : public al::ActorStateBase {
public:
    PlayerStateJump(al::LiveActor* player, const PlayerConst* pConst,
                    const IPlayerModelChanger* modelChanger, const IUseDimension* dimension,
                    const PlayerInput* input, const PlayerJumpMessageRequest* jumpMessageRequest,
                    const IJudge* judgeCapCatchJump, const PlayerCounterForceRun* counterForceRun,
                    const PlayerJointControlKeeper* jointControlKeeper,
                    const IUsePlayerCollision* collider, PlayerTrigger* trigger,
                    PlayerContinuousJump* continuousJump, PlayerAnimator* animator,
                    PlayerActionDiveInWater* actionDiveInWater, HackCap* hackCap,
                    IJudge* judgeWallCatch, bool isEnableCapJumpSlowly);

    void appear() override;

    f32 calcJumpPowerBorderSpeedMin() const;
    f32 calcJumpPowerBorderSpeedMax() const;
    f32 calcJumpPowerMin() const;
    f32 calcJumpPowerMax() const;
    f32 calcJumpGravity() const;
    void tryCountUpContinuousJump(PlayerContinuousJump* continuousJump);

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
    void updateNerveDownFall(const char* animName, f32 animFrame, f32 gravity, f32 fallSpeedMax,
                             const al::Nerve* nerve);
    void exeJumpSpinGround();
    void exeJumpSpinGroundDownFall();
    void exeJumpTurn();
    void exeJumpBack();
    void exeHoveringJump2D();
    void exeHovering2D();

private:
    const PlayerConst* mConst;
    const IPlayerModelChanger* mModelChanger;
    const IUseDimension* mDimension;
    const PlayerJumpMessageRequest* mJumpMessageRequest;
    const IJudge* mJudgeCapCatchJump;
    const PlayerCounterForceRun* mCounterForceRun;
    const PlayerJointControlKeeper* mJointControlKeeper;
    const IUsePlayerCollision* mCollider;
    PlayerAnimator* mAnimator;
    PlayerContinuousJump* mContinuousJump;
    PlayerTrigger* mTrigger;
    PlayerActionDiveInWater* mDiveInWater;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
    HackCap* mHackCap;
    IJudge* mJudgeWallCatch;
    bool mIsEnableCapJumpSlowly;
    s32 mExtendFrame = 0;
    f32 mJumpPower = 0.0f;
    f32 mMoveSpeedMax = 0.0f;
    f32 mJumpGravity = 0.0f;
    s32 _ac = 0;
    s32 mContinuousJumpCount = 0;
    bool mIsContinuousJump = false;
    bool _b5 = false;
    bool _b6 = false;
    bool _b7 = false;
    bool mIsJumpMessageRequest = false;
    bool _b9 = false;
    bool _ba = false;
    s32 _bc = 0;
    s32 _c0 = 0;
    s32 _c4 = 0;
    const char* mAnimName = nullptr;
    const char* mSubAnimName = nullptr;
    const PlayerInput* mInput;
    sead::Vector3f mTurnDir = {0.0f, 0.0f, 0.0f};
    s32 mHipDropAttackCount = 0;
    bool mIsDownFall = false;
    sead::Vector3f mDownFallStartPos = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(PlayerStateJump) == 0x100);
