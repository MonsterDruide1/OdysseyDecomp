#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
class JointSpringControllerHolder;
class RumbleCalculator;
}  // namespace al

class EquipmentInfo;
class KoopaCapPlayerBinder;
class PlayerEquipmentUser;

struct KoopaCapPlayerRumble {
    al::RumbleCalculator* left;
    al::RumbleCalculator* right;
    f32 handScaleL = 1.0f;
    f32 handScaleR = 1.0f;

    bool keepScaleL = false;
    bool keepScaleR = false;
};

static_assert(sizeof(KoopaCapPlayerRumble) == 0x20);

struct KoopaCapPlayerPunchState {
    enum class FinishState : s32 {
        Normal,
        Finish,
    };

    f32 fastPunchRate = 0.0f;
    al::LiveActor* focusTarget = nullptr;
    bool isPunchLeft = false;
    bool isFastPunchInput = false;
    bool isPunchHit = false;
    bool isPunchHitReaction = false;
    bool isPunchFollowUp = false;
    bool isTriggerSwingLeft = false;
    bool isTriggerSwingRight = false;
    bool isTriggerCapAction = false;
    s32 damageCooldown = 0;
    FinishState finishState = FinishState::Normal;
};

class KoopaCapPlayer : public al::LiveActor {
public:
    KoopaCapPlayer(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void startHideChase();
    void endEquipAndKill();
    bool isPlayingCatchDemo() const;
    bool isPlayerBinding() const;
    void onFinish();
    void offFinish();
    void endEquipAndBlowDown();
    void endEquipAndBlowDownWithoutHitReaction();

    void exeHideChase();
    void exeCatchPrepare();
    void exeCatch();
    void exeStart();
    void exeWait();
    void exeWaitBubble();
    void exePunchStart();
    void exePunchWait();
    void exePunch();
    void endPunch();
    void exePunchEnd();
    void exePunchFinishStart();
    void exePunchFinish();
    void exePunchFinishWait();
    void exePunchFinishEnd();
    void exeDamage();
    void exeBlowDown();
    void endBlowDown();

private:
    void updateTriggerInputs();

    KoopaCapPlayerRumble* getRumble() const { return mRumble; }

    KoopaCapPlayerPunchState mPunchState;
    KoopaCapPlayerBinder* mBinder = nullptr;
    const EquipmentInfo* mEquipmentInfo = nullptr;
    PlayerEquipmentUser* mEquipmentUser = nullptr;
    const sead::Vector3f* mHideChaseTrans = nullptr;
    KoopaCapPlayerRumble* mRumble = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mJointSpringControlRate = 1.0f;
    al::EnemyStateBlowDown* mBlowDownState = nullptr;
    f32 mCapBlowDownSideDegree = 0.0f;
    bool mIsAppearTutorialNoMovie = false;
};

static_assert(sizeof(KoopaCapPlayer) == 0x170);
