#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"
#include "Player/HackerStateBase.h"

class HackerJudgeStartJump;
class PlayerCounterForceRun;
class PlayerEyeSensorHitHolder;
class PlayerWallActionHistory;
class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerAnimator;
class PlayerConst;
class PlayerEffect;
class PlayerJumpMessageRequest;
class PlayerTrigger;
class YoshiActionTongueAttack;
class YoshiStateHackWait;
class YoshiStateHackJump;
class YoshiStateHackJumpFlap;
class YoshiStateHackRun;
class YoshiStateHackWallAir;
class YoshiStateHackWallPopUp;
class YoshiTongue;
class YoshiStateHackTongueShrink;
class YoshiStateHackFall;
class YoshiStateHackDown;
class HackerJudgeStartRun;
class YoshiJudgeFallFromGround;
class YoshiJudgeStartJumpFlap;
class YoshiJudgeStartTongueShrink;
class YoshiJudgeWallCling;
class YoshiJudgeWallHitDown;

namespace al {
class HitSensor;
class SensorMsg;
struct ActorInitInfo;
}  // namespace al

class YoshiStateHackPlay : public HackerStateBase {
public:
    explicit YoshiStateHackPlay(const al::ActorInitInfo&, al::LiveActor*, al::LiveActor*,
                                IUsePlayerHack**, const PlayerConst*, PlayerTrigger*,
                                PlayerAnimator*, IUsePlayerCollision*, PlayerEffect*,
                                PlayerJumpMessageRequest*);
    ~YoshiStateHackPlay() override;

    void appear() override;
    void kill() override;
    bool update() override;

    void attackSensor(al::HitSensor*, al::HitSensor*);
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

    void prepareEndHack();
    void updatePrevMovement();
    void updateAfterMovement();
    void startFruitShineGetDemo();

    void executePreUpdateNerveChange();
    void setNerveOnGround();
    void setNerveTongueShrink();
    bool tryChangeNerveAir();
    bool tryChangeNerveOnGround();

    void calcGroundPoseRate(f32*, f32*) const;
    bool tryGetLookAtTonguePos(sead::Vector3f*) const;
    bool tryCalcTonguePullForce(f32*, sead::Vector3f*) const;
    bool isActiveHeadCorrection() const;
    bool isCollisionShapeTongueJump() const;
    bool reactionCollidedCollisionCodeDamage() const;

    void exeWait();
    void exeRun();
    void exeJump();
    void exeFall();
    void exeFlap();
    void exeDown();
    void exeWallAir();
    void exeWallPopUp();
    void exeTongueShrink();

private:
    al::LiveActor* mYoshiModel = nullptr;
    const PlayerConst* mPlayerConst = nullptr;
    PlayerTrigger* mPlayerTrigger = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    IUsePlayerCollision* mUsePlayerCollision = nullptr;
    PlayerJumpMessageRequest* mPlayerJumpMessageRequest = nullptr;
    YoshiTongue* mYoshiTongue = nullptr;
    YoshiActionTongueAttack* mYoshiTongueAttack = nullptr;
    PlayerWallActionHistory* mWallActionHistory = nullptr;
    PlayerEyeSensorHitHolder* mEyeSensorHitHolder = nullptr;
    PlayerCounterForceRun* mPlayerCounterForceRun = nullptr;
    YoshiStateHackWait* mYoshiStateHackWait = nullptr;
    YoshiStateHackRun* mYoshiStateHackRun = nullptr;
    YoshiStateHackJump* mYoshiStateHackJump = nullptr;
    YoshiStateHackJumpFlap* mYoshiStateHackJumpFlap = nullptr;
    YoshiStateHackWallAir* mYoshiStateHackWallAir = nullptr;
    YoshiStateHackWallPopUp* mYoshiStateHackWallPopUp = nullptr;
    YoshiStateHackTongueShrink* mYoshiStateTongueShrink = nullptr;
    YoshiStateHackFall* mYoshiStateFall = nullptr;
    YoshiStateHackDown* mYoshiStateHackDown = nullptr;
    HackerJudgeStartJump* mJudgeStartJump = nullptr;
    HackerJudgeStartRun* mHackerJudgeStartRun = nullptr;
    YoshiJudgeFallFromGround* mYoshiJudgeFallFromGround = nullptr;
    YoshiJudgeStartJumpFlap* mYoshiJudgeStartJumpFlap = nullptr;
    YoshiJudgeStartTongueShrink* mJudgeStartTongueShrink = nullptr;
    YoshiJudgeWallCling* mYoshiJudgeWallCling = nullptr;
    YoshiJudgeWallHitDown* mYoshiJudgeWallHitDown = nullptr;
};

static_assert(sizeof(YoshiStateHackPlay) == 0x100);
