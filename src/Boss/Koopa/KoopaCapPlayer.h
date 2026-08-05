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
    f32 handScaleL;
    f32 handScaleR;

    union {
        struct {
            bool keepScaleL;
            bool keepScaleR;
        };

        u16 keepScaleFlags;
    };

    void clearKeepScale() { keepScaleFlags = 0; }
};

static_assert(sizeof(KoopaCapPlayerRumble) == 0x20);

struct KoopaCapPlayerPunchState {
    f32 fastPunchRate = 0.0f;
    u8 _4[4];
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
    s32 finishState = 0;

    bool hasTriggerSwingRightOrCapAction() const {
        if (isTriggerSwingRight)
            return true;
        return isTriggerCapAction;
    }

    bool hasTriggerCapActionOrSwingLeft() const {
        if (isTriggerCapAction)
            return true;
        return isTriggerSwingLeft;
    }

    bool trySetupNextPunchFromInput(bool allowFollowUp) {
        if (damageCooldown > 0)
            return false;

        bool nextPunchLeft;
        bool isFollowUp;
        bool isFastInput;
        if (isPunchLeft) {
            if (!hasTriggerSwingRightOrCapAction()) {
                if (!isTriggerSwingLeft)
                    return false;
                nextPunchLeft = true;
                isFollowUp = false;
                isFastInput = isTriggerSwingLeft;
            } else {
                nextPunchLeft = false;
                isFollowUp = allowFollowUp;
                isFastInput = isTriggerSwingRight;
            }
        } else if (hasTriggerCapActionOrSwingLeft()) {
            nextPunchLeft = true;
            isFollowUp = allowFollowUp;
            isFastInput = isTriggerSwingLeft;
        } else if (isTriggerSwingRight) {
            nextPunchLeft = false;
            isFollowUp = false;
            isFastInput = isTriggerSwingRight;
        } else {
            return false;
        }

        isFastPunchInput = isFastInput;
        isPunchFollowUp = isFollowUp;
        isPunchHit = false;
        isPunchHitReaction = false;
        isTriggerSwingLeft = false;
        isTriggerSwingRight = false;
        isTriggerCapAction = false;
        isPunchLeft = nextPunchLeft;
        return true;
    }
};

static_assert(sizeof(KoopaCapPlayerPunchState) == 0x20);

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

    void setHideChaseTransPtr(const sead::Vector3f* trans) { mHideChaseTrans = trans; }

    void setBinder(KoopaCapPlayerBinder* binder) { mBinder = binder; }

    void setFastPunchRate(f32 rate) { mPunchState.fastPunchRate = rate; }

    void setAppearTutorialNoMovie(bool value) { mIsAppearTutorialNoMovie = value; }

    KoopaCapPlayerPunchState mPunchState;
    KoopaCapPlayerBinder* mBinder = nullptr;
    const EquipmentInfo* mEquipmentInfo = nullptr;
    PlayerEquipmentUser* mEquipmentUser = nullptr;
    const sead::Vector3f* mHideChaseTrans = nullptr;
    KoopaCapPlayerRumble* mRumble = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mJointSpringControlRate = 1.0f;
    u8 _15c[4];
    al::EnemyStateBlowDown* mBlowDownState = nullptr;
    f32 mCapBlowDownSideDegree = 0.0f;
    bool mIsAppearTutorialNoMovie = false;
    u8 _16d[3];
};

static_assert(sizeof(KoopaCapPlayer) == 0x170);
