#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class CapTargetInfo;
class HackerDepthShadowMapCtrl;
class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerAnimator;
class PlayerConst;
class PlayerEffect;
class PlayerHackStartShaderCtrl;
class PlayerJumpMessageRequest;
class PlayerModelChangerYoshi;
class PlayerTrigger;
class YoshiStateHackPlay;

namespace al {
class HitSensor;
class SensorMsg;
class WaterSurfaceFinder;
struct ActorInitInfo;
}  // namespace al

class YoshiStateHack : public al::ActorStateBase {
public:
    explicit YoshiStateHack(const al::ActorInitInfo&, al::LiveActor*, al::LiveActor*,
                            const CapTargetInfo*, const PlayerConst*, const al::WaterSurfaceFinder*,
                            PlayerTrigger*, PlayerModelChangerYoshi*, PlayerAnimator*,
                            IUsePlayerCollision*, PlayerEffect*, PlayerJumpMessageRequest*);
    ~YoshiStateHack() override;

    void appear() override;

    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsgEndState(bool*, const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool receiveMsgSetNerveState(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

    void prepareEndHack();
    void updatePrevMovement();
    void updateAfterMovement();
    void startFruitShineGetDemo();
    bool reactionCollidedCollisionCode();

    void calcGroundPoseRate(f32*, f32*) const;
    bool tryGetLookAtTonguePos(sead::Vector3f*) const;
    bool tryCalcTonguePullForce(f32*, sead::Vector3f*) const;
    bool isActiveHeadCorrection() const;
    bool isEnableUpdateCollider() const;
    bool isCollisionShapeTongueJump() const;

    void exeHack();
    void exeDemo();
    void exeDemoHackStart();
    void exeLockOn();

private:
    al::LiveActor* mYoshiActor = nullptr;
    const PlayerConst* mPlayerConst = nullptr;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    IUsePlayerCollision* mPlayerCollisionUser = nullptr;
    const CapTargetInfo* mCapTargetInfo = nullptr;
    PlayerModelChangerYoshi* mModelChanger = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    IUsePlayerHack* mHackUser = nullptr;
    YoshiStateHackPlay* mStateHackPlay = nullptr;
    PlayerHackStartShaderCtrl* mHackStartShaderCtrl = nullptr;
    HackerDepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};

static_assert(sizeof(YoshiStateHack) == 0x78);
