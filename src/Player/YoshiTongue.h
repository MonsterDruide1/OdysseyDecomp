#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class IUsePlayerHack;
class PlayerConst;
class PlayerEyeSensorHitHolder;
class PlayerWallActionHistory;
class IUsePlayerCollision;

namespace al {
struct ActorParamS32;
struct ActorParamF32;
class CollisionParts;
}  // namespace al

struct YoshiTongueParam {
    al::ActorParamF32* maximumSpeed;   // maximum stretching speed of the tongue
    al::ActorParamS32* brakeTime;      // how long the tongue stays stretched out for
    al::ActorParamF32* reachDistance;  // how far the tongue should stretch
    al::ActorParamS32* endpointStopTime;
    al::ActorParamS32* returnFrame;
    al::ActorParamF32* returnStrength;
    al::ActorParamF32* maximumReturnSpeed;
};

class YoshiTongue : public al::LiveActor {
public:
    YoshiTongue(const al::LiveActor* yoshi, const al::LiveActor* yoshiModelActor,
                const IUsePlayerCollision*, const PlayerWallActionHistory*,
                const PlayerEyeSensorHitHolder*, const PlayerConst*, IUsePlayerHack**, const char*);

    void init(const al::ActorInitInfo&) override;
    void calcAnim() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    void updateCollider() override;

    void startAttack(const sead::Vector3f& front, const sead::Vector3f& up);
    void startShrink();
    void endShrink();
    void endHack();
    void eatFinish();
    void returnOrEatHide();
    void updateEatBindActor();
    void adjustShrinkRestRange(float);

    bool reactionCollideWall();
    bool reactionCollideGround();

    bool isShrinkMove() const;
    bool isConnectWall() const;
    bool isConnectGround() const;
    bool isExistEatBind() const;
    bool isEnableEatFinish() const;
    bool isEnableLookAtTip() const;
    bool isEnableShrinkStart() const;
    bool isEnableStartAttack() const;
    bool isEnableStayClingGround() const;

    void calcTongueTipPos(sead::Vector3f*) const;
    void calcYoshiFaceDir(sead::Vector3f*) const;

    bool tryCalcTongueConnect(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, sead::Vector3f*) const;
    bool tryCalcTonguePullForce(float*, sead::Vector3f*) const;
    bool tryCalcTonguePullDistance(sead::Vector3f*) const;

    float getShrinkRestRange() const;
    float getTongueParamRange() const;
    float getTongueParamSpeed() const;

    void exeStay();
    void exeStretch();
    void exeShrink();
    void exeReturn();
    void exeHide();
    void exeHit();
    void exeEat();
    void exeClingWall();
    void exeClingGround();

private:
    const al::LiveActor* mYoshi = nullptr;
    const al::LiveActor* mYoshiModel = nullptr;
    char mPaddingToParam[0x48];
    YoshiTongueParam* mParam;
    char mPadding[0x1b8 - sizeof(al::LiveActor)];
};

static_assert(sizeof(YoshiTongue) == 0x218);
