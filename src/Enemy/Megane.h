#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
class CollisionPartsFilterSpecialPurpose;
class CameraPoserSubjective;
class CameraTicket;
class NerveKeeper;
}  // namespace al

class EnemyStateSwoon;
class TestFilterGlasses;
class IUsePlayerHack;
class PlayerPushReceiver;
class CapTargetInfo;
class HackerStateNormalMove;
class PlayerHackStartShaderCtrl;

class Megane : public al::LiveActor {
public:
    Megane(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void updateCollider() override;
    void control() override;

    void notifyKillSwitch();
    bool isNerveDieOrRebirth() const;
    bool isNormalNerveWithWaitHack() const;
    bool isNormalNerveWithoutWaitHack() const;
    bool tryReceiveMsgKillByShineGet(const al::SensorMsg* message);
    void prepareHack(al::HitSensor* self, al::HitSensor* other);
    void endCameraSubjective();
    bool isHack() const;
    bool isEnableTrample(al::HitSensor* self, al::HitSensor* other) const;
    void updateMovement();
    bool tryShiftFall();
    void updateRunAwayDirection();
    void shiftWaitOrRunAway();
    bool isTriggerStare() const;
    bool isTriggerSubjective() const;
    void startCameraSubjective();
    void faceToCameraFront();
    void tryToggleSpectaclesNerve();
    void shiftHackNerveOnGround();
    void putOnSpectaclesForce();
    bool isWearingGlasses() const;
    void updateDither();
    void recordWallNormal();

    void exeWait();
    void exeFind();
    void exeRunAway();
    void exeFall();
    void exeLand();
    void exeTrampled();
    void exeWaitHack();
    void exeHackStart();
    void exeHack();
    void exeHackSlow();
    void exeHackSubjective();
    void exeHackFall();
    void exeHackLand();
    void exeDie();
    void exeRebirth();
    void exeAppear();
    void exeSpectaclesOff();
    void exeSpectaclesOn();

private:
    sead::Quatf mStartingQuat = {0.0f, 0.0f, 0.0f, 1.0f};
    sead::Vector3f mStartingPos = {0.0f, 0.0f, 0.0f};
    CapTargetInfo* mCapTargetInfo = nullptr;
    IUsePlayerHack* mPlayerHack = nullptr;
    EnemyStateSwoon* mStateSwoon = nullptr;
    HackerStateNormalMove* mHackerState = nullptr;
    HackerStateNormalMove* mHackerStateSpectacles = nullptr;
    PlayerPushReceiver* mPlayerPushReceiver = nullptr;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
    TestFilterGlasses* mTestFilterGlasses = nullptr;

    s32 mGlassFilterDelay = 0;
    s32 mWaitDelay = 0;
    bool mIsHack = false;
    al::CollisionPartsFilterSpecialPurpose* mCollisionPartsFilterBase = nullptr;
    al::CameraPoserSubjective* mCameraPoserSubjective = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    f32 mModelAlphaMask = 1.0f;

    sead::Vector3f mPlayerPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mWallNormal = {0.0f, 0.0f, 0.0f};
    u32 mRunAwayTime = 0;
    al::HitSensor* mCapReceiverSensor = nullptr;
    bool mIsHackTutorialVisible = false;
    al::NerveKeeper* mNerveKeeper = nullptr;
};
