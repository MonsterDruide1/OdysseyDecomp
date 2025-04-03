#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class IUseNerve;
class MtxConnector;
}  // namespace al

class CoinStateAppearRotate;
class CoinRotateCalculator;
class WaterSurfaceShadow;
class SaveObjInfo;
class ExternalForceKeeper;

class Coin : public al::LiveActor {
public:
    Coin(const char* name, bool isDemo = false);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void appear() override;
    void makeActorAlive() override;
    void control() override;
    void endClipped() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void tryCreateMtxConnector();
    void appearCirclePlacement();
    void appearPopUpCommon(bool startHitReaction);
    void appearPopUp();
    void appearPopUpWithoutHitReaction();
    void appearPopUpVelocity();
    void appearAbove();
    void appearCountUpCommon(sead::Vector3f* velocity, s32 delay);
    void appearCountUp();
    void appearCountUpFiveCount();
    void appearCountUp3(s32 coinIndex);
    void appearCountUp5(s32 coinIndex);
    void appearCountUp10(s32 coinIndex);
    void appearCountUpFixPos10(s32 coinIndex);
    void appearCoinRail();
    void appearCoinChameleon(const sead::Vector3f& trans, const sead::Quatf& quat,
                             const sead::Vector3f& position);
    void appearLimitTime(s32 timeLimit);
    void appearBlow(const sead::Vector3f& velocity, s32 delayTime);
    void rotate();
    void appearBlowUpCommon(s32 delayTime, f32 horizontalForce, f32 verticalForce, s32 coinCount,
                            s32 coinIndex);
    void appearBlowUp(s32 coinIndex, s32 coinCount);
    void appearBlowUpLittle(s32 coinIndex, s32 coinCount);
    void appearFall(const sead::Vector3f& velocity, s32 timeLimit);
    void appearPlayerDead(const sead::Vector3f& position, const sead::Vector3f& direction,
                          bool isInWater, bool isTimeBalloon);
    void appearPlayerDeadReplace(const sead::Vector3f& position);
    void setShadowDropLength(f32 shadowLength);
    void get();

    bool isGot() const;
    bool isGotOrRotate() const;
    bool isWait() const;
    bool isAnyWaitOrAppear() const;

    void exeAppear();
    void exeAppearCoinLead();
    void exePopUp();
    void exeCountUpDelay();
    void exeCountUp();
    void exeWait();
    void exeWaitOnDemoEnd();
    void exeGot();
    void exeBlowUpDelay();

    void setMtxConnector(al::MtxConnector* mtxConnector) { mMtxConnector = mtxConnector; }

private:
    CoinStateAppearRotate* mStateAppearRotate = nullptr;
    sead::Vector3f mDisplayOffset = sead::Vector3f::zero;
    sead::Quatf mStartingQuat = sead::Quatf::unit;
    al::MtxConnector* mMtxConnector = nullptr;
    bool mIsConnectToCollisionBack = false;
    s32 mTimeLimit = -1;
    sead::Vector3f mChameleonOffset = sead::Vector3f::zero;
    ExternalForceKeeper* mExternalForceKeeper = nullptr;
    CoinRotateCalculator* mRotateCalculator = nullptr;
    bool mIsPlaced = false;
    f32 mShadowSize = 0.0f;
    sead::Vector3f mPoseTrans = sead::Vector3f::zero;
    sead::Quatf mPoseQuat = sead::Quatf::unit;
    WaterSurfaceShadow* mWaterSurfaceShadow = nullptr;
    s32 mCountUpDelay = 0;
    sead::Vector3f mBlowVelocity = sead::Vector3f::zero;
    SaveObjInfo* mSaveObjInfo = nullptr;
    bool mIsDemo;
};

namespace CoinFunction {

const sead::Vector3f& getAppearAboveVelocity();
void appearCoinBlowVeryLittle(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowLittle(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowMiddle(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowLarge(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowLargeEnemy(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowBoss(Coin* coin, const sead::Vector3f& direction);
void appearCoinBlowGk(Coin* coin, const sead::Vector3f& direction);

}  // namespace CoinFunction
