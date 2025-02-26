#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class AreaObjGroup;
class ActorCameraTarget;
class CameraTicket;
class HitSensor;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al
class Shine;

class KuriboGirl : public al::LiveActor {
public:
    KuriboGirl(const char* name);
    void init(const al::ActorInitInfo& actorInitInfo) override;
    void control() override;
    void exeWait();
    void lookFront();
    void exeFind();
    void lookPlayer();
    void exeLoveLook();
    void exeLoveLookTurn();
    void exeSurprise();
    void exeDisappear();
    void exeHide();
    void exeAppear();
    void exePreLoveFind();
    void exeLoveFind();
    void exeAppearShineStart();
    void exeAppearShine();
    void exeAppearShineEnd();
    void exeLookLoveAfter();
    void exeWaitLoveAfter();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool isWatchStart(sead::Vector3f pos);
    bool isNrvWait();
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;

private:
    al::AreaObjGroup* mAreaObjGroup = nullptr;
    bool mIsAfterShineAppear = false;
    bool mIsPlayerNear = false;
    bool mIsLoveLook = false;
    bool mIsPlayerFar = true;
    s32 mFramesOnGround = 0;
    al::CameraTicket* mCamera = nullptr;
    al::ActorCameraTarget* mCameraTarget = nullptr;
    Shine* mShineActor = nullptr;
    sead::Vector3f* mShineMoveDir = nullptr;
    bool mIsMoveShine = false;
    sead::Vector3f mHitSensorPos = sead::Vector3f::zero;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mHeadRotator = 0.0f;
    sead::Vector3f mInitialFront = sead::Vector3f::zero;
};

static_assert(sizeof(KuriboGirl) == 0x160);
