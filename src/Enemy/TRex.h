#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Player/IUsePlayerCollision.h"

namespace al {
struct ActorInitInfo;
class ActorCameraSubTarget;
class ActorCameraTarget;
class AreaObjGroup;
class CameraTicket;
class HitSensor;
class IBgmParamsChanger;
class IUseNerve;
class JointSpringControllerHolder;
class LiveActorGroup;
class Nerve;
class SensorMsg;
}  // namespace al

class CapTargetInfo;
class CollisionShapeInfoSphere;
class EnemyCap;
class EnemyStateHackStart;
class IUsePlayerHack;
class PlayerCollider;
class TRex;
class TRexCollisionAnalyzer;
class TRexFootIK;
class TRexGroundHeightCtrl;
class TRexNeckAnimInterpoleCtrl;
class TRexNeckDynamicsCtrl;
class TRexNeckRumbleCtrl;
class TRexNeckSwingWatchCtrl;
class TRexNeckUpswingCtrl;
class TRexPartialAnimCtrl;
class TRexPatrolRouteRider;
class TRexSmallGroundJointAnimator;
class TRexStatePatrolChase;
class TRexStateSleep;

class TRexHackTurnInfo {
public:
    TRexHackTurnInfo();

    void resetSpeed();
    void updateWithInput();
    void updateNoInput();
    void setReverseInput();
    bool isExistPreInput() const;
    bool isInvalidUpdateTurnSpeed() const;
    f32 calcInputScale() const;

private:
    s32 mTurnSpeed = 0;  // TODO: verify
    s32 _4 = 0;
    s32 mPreInputCount = 0;
    s32 mReverseInputTimer = 0;
    s32 _10 = 0;
    s32 _14 = 0;
    s32 mSpeed = 0;  // TODO: verify
    s32 _1c = 0;
};

static_assert(sizeof(TRexHackTurnInfo) == 0x20);

struct TRexHackTimeUpInfo {
    al::LiveActor* actor = nullptr;
    s32 timer = 0;
    const char* currentSeName = nullptr;
};

static_assert(sizeof(TRexHackTimeUpInfo) == 0x18);

struct TRexCollidedShapeInfo {
    const TRex* tRex = nullptr;
    sead::PtrArrayImpl* collidedShapes = nullptr;
    s32 _10 = 0;
};

static_assert(sizeof(TRexCollidedShapeInfo) == 0x18);

class TRex : public al::LiveActor, public IUsePlayerCollision {
public:
    TRex(const char* name);

    void init(const al::ActorInitInfo& info) override;
    const TRexFootIK* getLeftFootIK() const;
    const TRexFootIK* getRightFootIK() const;
    void initAfterPlacement() override;
    void appear() override;
    void kill() override;
    void movement() override;
    void control() override;
    void updateCollider() override;
    void calcAnim() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeSleep();
    void endSleep();
    void exeSwoonStart();
    void exeSwoonStartLand();
    void exeSwoon();
    void exeFall();
    void exeLand();
    void exeHackStart();
    void exeHackWait();
    void exeHackAttackWait();
    void endHackAttackWait();
    void exeHackMove();
    void exeHackMoveAfter();
    void exeHackMoveGround();
    void exeHackMoveJump();
    void exeHackMoveEnd();
    void exeHackHoldJump();
    void exeHackFall();
    void exeHackJumpStart();
    void exeHackJump();
    void exeHackTurn();
    void exeHackTurnEnd();
    void exeHackLand();
    void endHackLand();
    void exeReset();
    void resetAllJointController();
    void exePatrolWalkStart();
    void exePatrolWalk();
    void exePatrolWalkAfter();
    void exePatrolWalkGround();
    void exePatrolWalkEndL();
    void exePatrolWaitSniff();
    void exePatrolNoticeSniff();
    void exePatrolTurnMario();
    void exePatrolTurnBack();
    void exePatrolDash();
    void exePatrolHoldJump();
    void exePatrolFall();
    void exePatrolLand();
    void exePatrolSwoonStartDashClash();
    void exePatrolSwoonStart();
    void exePatrolSwoon();
    void exePatrolSwoonEnd();
    void exePatrolReset();
    bool isEnableBreakPartsForceScroll() const;
    f32 getMaxSinkGroundHeight();
    f32 getHoldJumpStartUpSpeed();
    f32 getHoldJumpMoveAccel();
    f32 getPatrolGravityAccel();
    f32 getPatrolGroundFriction();
    f32 getMaxPatrolFallSpeedOnGround();
    const char* getHeadColliderShapeName();
    void exeForceScrollStartFall();
    void exeForceScrollStartLand();
    void exeForceScrollRunStart();
    void exeForceScrollRun();
    void exeForceScrollAttackSign();
    void exeForceScrollAttackStart();
    void exeForceScrollAttack();
    void endForceScrollAttack();
    void exeForceScrollAttackEnd();
    void exeForceScrollEndFallStart();
    void exeForceScrollEndFall();

    PlayerCollider* getPlayerCollider() const override { return mPlayerCollider; }

    bool isForceScrollEndFallStart() const { return mIsForceScrollEndFallStart; }

private:
    s32 mParamType = 0;
    s32 mTRexType = 1;
    TRexFootIK** mFootIKs = nullptr;
    TRexHackTimeUpInfo* mHackTimeUpInfo = nullptr;
    TRexGroundHeightCtrl* mGroundHeightCtrl = nullptr;
    TRexNeckDynamicsCtrl* mNeckDynamicsCtrl = nullptr;
    TRexNeckUpswingCtrl* mNeckUpswingCtrl = nullptr;
    TRexNeckSwingWatchCtrl* mNeckSwingWatchCtrl = nullptr;
    TRexNeckRumbleCtrl* mNeckRumbleCtrl = nullptr;
    TRexNeckAnimInterpoleCtrl* mNeckAnimInterpoleCtrl = nullptr;
    TRexSmallGroundJointAnimator* mSmallGroundJointAnimator = nullptr;
    TRexPartialAnimCtrl* mPartialAnimCtrl = nullptr;
    al::LiveActorGroup* mFootPrintGroup = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    IUsePlayerHack* mPlayerHack = nullptr;
    TRexHackTurnInfo* mHackTurnInfo = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyCap* mEnemyCap = nullptr;
    EnemyStateHackStart* mStateHackStart = nullptr;
    TRexStateSleep* mStateSleep = nullptr;
    TRexStatePatrolChase* mStatePatrolChase = nullptr;
    TRexCollidedShapeInfo* mCollidedShapeInfo = nullptr;
    TRexCollisionAnalyzer* mCollisionAnalyzer = nullptr;
    PlayerCollider* mPlayerCollider = nullptr;
    CollisionShapeInfoSphere* mHeadShapeInfo = nullptr;
    CollisionShapeInfoSphere* mBodyShapeInfo = nullptr;
    al::LiveActor* mHeadCollisionObj = nullptr;
    al::CameraTicket* mHackStartCameraTicket = nullptr;
    al::CameraTicket* mFollowLimitCameraTicket = nullptr;
    al::ActorCameraTarget* mCameraTarget = nullptr;
    al::ActorCameraSubTarget* mCameraSubTarget = nullptr;
    sead::Vector3f mCameraSubTargetTrans = sead::Vector3f::zero;
    sead::Matrix34f mWaterSurfaceBodyMtx = sead::Matrix34f::ident;
    sead::Matrix34f mWaterSurfaceLMtx = sead::Matrix34f::ident;
    sead::Matrix34f mWaterSurfaceRMtx = sead::Matrix34f::ident;
    u8 _29c[4] = {};
    al::AreaObjGroup* mWaterfallArea = nullptr;
    bool mIsHackStartCameraActive = true;
    sead::Vector3f mInitFront = sead::Vector3f::zero;
    sead::Vector3f* mHackStartAfterPostureFront = nullptr;
    sead::Vector3f mGroundNormal = sead::Vector3f::ey;
    sead::Vector3f mCollisionPushOffset = sead::Vector3f::zero;
    sead::Vector3f mTailSensorPos = sead::Vector3f::zero;
    sead::Vector3f mRouteHeadGuidePos = sead::Vector3f::zero;
    s32 mHackMoveCounter = 0;
    sead::Vector3f mHackTurnPrevDir = sead::Vector3f::zero;
    sead::Vector3f mHackTurnTargetDir = sead::Vector3f::zero;
    f32 mPatrolTurnAngle = 0.0f;
    f32 mJumpStartY = 0.0f;
    f32 mAllRootLocalZRot = 0.0f;
    bool mIsInWater = false;
    sead::Vector3f mForceScrollMoveOffset = sead::Vector3f::zero;
    f32 mMaxSideOffset = 1000.0f;
    bool mIsForceScrollEndFallStart = false;
    sead::PtrArray<al::IBgmParamsChanger>* mBgmParamsChangers = nullptr;
    al::LiveActorGroup* mBirdGroup = nullptr;
    TRexPatrolRouteRider* mPatrolRouteRider = nullptr;
    s32 mNearPlayerCount = 0;
    s32 mPatrolTurnRetryCount = 0;
    void* _350 = nullptr;
    s32 _358 = 0;
    s32 mGamaneBulletHitCount = 0;
};

static_assert(sizeof(TRex) == 0x360);

namespace TRexFunction {
void sendMsgTRexAttackToAllCollidedCollision(const TRex* rex);
void updateMaterialCodeByFootCollide(TRex* rex, const TRexFootIK* footIK);
void appearFootPrint(al::LiveActorGroup* group, const TRex* rex, bool isLeft);
void updatePatrolRouteRider(TRexPatrolRouteRider* rider, const TRex* rex);
bool executePatrolTurn(al::IUseNerve* nerveKeeper, TRex* rex, sead::Vector3f* prevDir,
                       sead::Vector3f* targetDir);
f32 calcBlendAnimFrameRateByIndex(const TRex* rex, s32 index);
bool tryStartPatrolTurnPoint(al::IUseNerve* nerveKeeper, const TRex* rex, const al::Nerve* nerve,
                             sead::Vector3f* turnDir, sead::Vector3f* prevDir,
                             const sead::Vector3f& target, f32 angleDegree);
void updatePatrolRunSpeed(TRex* rex, sead::Vector3f* moveVec);
void updatePatrolDashSpeed(TRex* rex, sead::Vector3f* moveVec, f32 speedScale);
void updatePatrolFindTurn(TRexHackTurnInfo* turnInfo, TRex* rex, const sead::Vector3f& dir,
                          f32 turnSpeed, const TRexCollisionAnalyzer* collisionAnalyzer);
void updateFallPoseAndVelocity(TRex* rex, f32 gravityScale);
}  // namespace TRexFunction
