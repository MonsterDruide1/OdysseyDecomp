#pragma once

#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class HitSensor;
class JointLookAtController;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al

class BossForestBarrierCtrl;
class BossForestBattlePhase;
class BossForestCameraController;
class SenobiGeneratePoint;
class BossForestSatelliteController;
class BossForestSatelliteHolder;
class BossForestStateDemoBattleStart;
class BossForestStateWaitBattleStart;

class BossForest : public al::LiveActor, public IUseDemoSkip {
public:
    BossForest(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other) override;

    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void updateOnlyDemoGraphics() override;

    void updateSatellitesPose(const sead::Quatf& quat);
    BossForestSatelliteController* getCurrentSatelliteController() const;
    SenobiGeneratePoint* getSenobiGeneratePoint(s32 index) const;
    const sead::PtrArray<al::LiveActor>& getFieldWalls();
    bool isStateBodyAttack() const;
    void setStateDemoBattleEnd();
    sead::Matrix34f* getLifePartsEffectMtx(const char* name);

    bool isBattleLv2() const { return mIsLv2 != 0; }

    BossForestBarrierCtrl* getBarrierCtrl() const { return mBarrierCtrl; }

    BossForestCameraController* getCameraController() const { return mCameraController; }

    BossForestSatelliteHolder* getSatelliteHolder() const { return mSatelliteHolder; }

    const sead::Vector3f& getInitTrans() const { return mInitTrans; }

    const sead::Vector3f& getBattleEyePos() const { return mBattleEyePos; }

    al::JointLookAtController* getJointLookAtController() const { return mJointLookAtController; }

    al::JointSpringControllerHolder* getJointSpringControllerHolder() const {
        return mJointSpringControllerHolder;
    }

    void setHasCollisionParts(bool hasCollisionParts) { mHasCollisionParts = hasCollisionParts; }

    bool hasCollisionParts() const { return mHasCollisionParts; }

    sead::Matrix34f* getAttackRingMtxPtr() { return &mAttackRingMtx; }

    void exeWaitForBattleStart();
    void exeDemoBattleStart();
    void exeBattlePhase1();
    void exeBattlePhase2();
    void exeBattlePhase3();
    void exeDemoBattleEnd();
    void exeDie();

private:
    u8 mIsLv2 = 0;
    BossForestStateWaitBattleStart* mStateWaitBattleStart = nullptr;
    BossForestStateDemoBattleStart* mStateDemoBattleStart = nullptr;
    BossForestBattlePhase* mBattlePhase1 = nullptr;
    BossForestBattlePhase* mBattlePhase2 = nullptr;
    BossForestBattlePhase* mBattlePhase3 = nullptr;
    BossForestBarrierCtrl* mBarrierCtrl = nullptr;
    BossForestCameraController* mCameraController = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    BossForestSatelliteHolder* mSatelliteHolder = nullptr;
    sead::PtrArray<SenobiGeneratePoint> mSenobiGeneratePoints;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Vector3f mBattleEyePos = sead::Vector3f::zero;
    sead::Vector3f mCameraTargetPos = sead::Vector3f::zero;
    al::JointLookAtController* mJointLookAtController = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    bool mIsHintShown = false;
    bool mHasCollisionParts = false;
    sead::Matrix34f mCollisionMtx = sead::Matrix34f::ident;
    sead::FixedObjArray<sead::Matrix34f, 3> mLifePartsMtxArray;
    sead::Matrix34f mAttackRingMtx = sead::Matrix34f::ident;
    sead::PtrArray<al::LiveActor> mFieldWalls;
};

static_assert(sizeof(BossForest) == 0x2E8);
