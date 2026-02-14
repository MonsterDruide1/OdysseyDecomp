#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
class HitSensor;
}  // namespace al

class PlayerHackKeeper;
class IPlayerModelChanger;
class PlayerAnimator;
class HackCap;

class PlayerStateHack : public al::ActorStateBase {
public:
    PlayerStateHack(al::LiveActor* player, PlayerHackKeeper* hackKeeper,
                    IPlayerModelChanger* modelChanger, PlayerAnimator* animator, HackCap* hackCap);

    void appear() override;
    void prepareStartHack(const al::HitSensor* source, const al::HitSensor* target);
    void prepareEndHack();
    void prepareStageStartHack();
    bool isIgnoreUpdateCollider() const;
    bool isEnableChangeState() const;
    bool isEnableModelSyncShowHide() const;
    bool isEnableCancelHack() const;
    void exeHackDemo();
    void exeHackDemoPuppetable();
    void exeHack();
    void exeEnd();

private:
    PlayerHackKeeper* mHackKeeper;
    IPlayerModelChanger* mModelChanger;
    PlayerAnimator* mAnimator;
    HackCap* mHackCap;
    bool mIsStageStartHack = false;
    s32 mHackDemoStartLength = 0;
    sead::Vector3f mStartTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mStartSensorTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mCurSensorTrans = {0.0f, 0.0f, 0.0f};
    sead::Matrix34f mPossessTraceMtx = sead::Matrix34f::ident;
    sead::Matrix34f mDemoModelMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(PlayerStateHack) == 0xd0);
