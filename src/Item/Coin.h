#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

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
    void appearBlow(const sead::Vector3f& direction, s32 delayTime);
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

private:
    char _108[0xA8];
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
