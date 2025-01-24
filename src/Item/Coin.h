#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class HitSensor;
class MtxConnector;
}  // namespace al

class CoinStateAppearRotate;
class CoinRotateCalculator;
class WaterSurfaceShadow;
class SaveObjInfo;
class ExternalForceKeeper;

class Coin : public al::LiveActor {
public:
    Coin(const char* name, bool);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void appear() override;
    void makeActorAlive() override;
    void control() override;
    void endClipped() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void tryCreateMtxConnector();
    bool isGot() const;
    void appearCirclePlacement();
    void appearPopUp();
    void appearPopUpWithoutHitReaction();
    void appearPopUpVelocity();
    void appearAbove();
    void appearCountUp();
    void appearCountUpFiveCount();
    void appearCountUp3(s32);
    void appearCountUp5(s32);
    void appearCountUp10(s32);
    void appearCountUpFixPos10(s32);
    void appearCoinRail();
    void appearCoinChameleon(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
    void appearLimitTime(s32);
    void appearBlow(const sead::Vector3f& velocity, s32 delayTime);
    void rotate();
    void appearBlowUp(s32, s32);
    void appearBlowUpCommon(s32, f32, f32, s32, s32);
    void appearBlowUpLittle(s32, s32);
    void appearFall(const sead::Vector3f&, s32);
    void appearPlayerDead(const sead::Vector3f&, const sead::Vector3f&, bool, bool);
    void appearPlayerDeadReplace(const sead::Vector3f&);
    void setShadowDropLength(f32);
    void get();
    bool isGotOrRotate() const;
    bool isWait() const;

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
    CoinStateAppearRotate* mStateAppearRotate;
    char _110[0xc];
    sead::Quatf _11c;
    al::MtxConnector* mMtxConnector;
    bool _138;
    s32 _13c;
    sead::Vector3f _140;
    char _14c;
    ExternalForceKeeper* mExternalForceKeeper;
    CoinRotateCalculator* mRotateCalculator;
    bool _160;
    f32 _164;
    sead::Vector3f _168;
    sead::Quatf _174;
    char _184[4];
    WaterSurfaceShadow* mWaterSurfaceShadow;
    s32 _190;
    sead::Vector3f _194;
    SaveObjInfo* mSaveObjInfo;
    bool _1a8;
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
