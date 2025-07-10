#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

#include "Player/IUsePlayerCollision.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class CollisionShapeKeeper;
class HackerDepthShadowMapCtrl;
class HackerJudgeNormalFall;
class HackerJudgeStartRun;
class HackerStateWingFly;
class IUsePlayerHack;
class PlayerActionTurnControl;
class PlayerCollider;

class KaronWingStateHack : public al::ActorStateBase, public IUsePlayerCollision {
public:
    KaronWingStateHack(al::LiveActor* parent, const al::ActorInitInfo& info,
                       IUsePlayerHack** playerHack);

    void appear() override;
    void kill() override;
    void control() override;
    PlayerCollider* getPlayerCollider() const override;

    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void updateCollider();
    void resetFlyLimit(const sead::Vector3f& flyLimit);
    bool isEndCancel() const;
    bool isEndReset() const;
    bool isEndDamage() const;
    void updateBasePos();
    bool tryFly();
    bool tryLanding();

    void exeWait();
    void exeWalk();
    void exeLand();
    void exeWingFly();

    void exeEndCancel() {}

    void exeEndReset() {}

    void exeEndDamage() {}

private:
    IUsePlayerHack** mPlayerHack = nullptr;
    PlayerActionTurnControl* mPlayerActionTurnControl = nullptr;
    PlayerCollider* mPlayerCollider = nullptr;
    CollisionShapeKeeper* mCollisionShapeKeeper = nullptr;
    HackerJudgeNormalFall* mJudgeNormalFall = nullptr;
    HackerJudgeStartRun* mJudgeStartRun = nullptr;
    HackerStateWingFly* mStateWingFly = nullptr;
    f32 mShadowMaskDropLength = 0.0f;
    HackerDepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};
