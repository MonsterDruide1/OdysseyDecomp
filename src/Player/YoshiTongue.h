#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class IUsePlayerCollision;
class PlayerWallActionHistory;
class PlayerEyeSensorHitHolder;
class PlayerConst;
class IUsePlayerHack;
class IUsePlayerCollision;

class YoshiTongue : public al::LiveActor {
public:
    YoshiTongue(const al::LiveActor*, const al::LiveActor*, const IUsePlayerCollision*,
                const PlayerWallActionHistory*, const PlayerEyeSensorHitHolder*, const PlayerConst*,
                IUsePlayerHack**, const char*);

    void init(const al::ActorInitInfo& info) override;
    void updateCollider() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void updateEatBindActor();
    void calcAnim() override;
    void startAttack(const sead::Vector3f&, const sead::Vector3f&);
    void startShrink();
    void endShrink();
    void eatFinish();
    void endHack();
    bool isEnableStartAttack() const;
    bool isEnableLookAtTip() const;
    bool isEnableShrinkStart() const;
    bool isEnableEatFinish() const;
    bool isExistEatBind() const;
    bool isShrinkMove() const;
    bool isConnectWall() const;
    bool isConnectGround() const;
    void calcYoshiFaceDir(sead::Vector3f*) const;
    void calcTongueTipPos(sead::Vector3f*) const;
    bool tryCalcTonguePullForce(f32*, sead::Vector3f*) const;
    bool tryCalcTonguePullDistance(sead::Vector3f*) const;
    bool tryCalcTongueConnect(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, sead::Vector3f*) const;
    f32 getShrinkRestRange() const;
    void adjustShrinkRestRange(f32);
    void exeStretch();
    f32 getTongueParamSpeed() const;
    f32 getTongueParamRange() const;
    bool reactionCollideWall();
    bool reactionCollideGround();

    void returnOrEatHide();
    void exeStay();
    void exeHit();
    void exeClingWall();
    void exeClingGround();
    void exeShrink();
    void exeReturn();

    void exeEat();
    void exeHide();
    bool isEnableStayClingGround() const;

private:
    char filler[0x110];
};

static_assert(sizeof(YoshiTongue) == 0x218);
