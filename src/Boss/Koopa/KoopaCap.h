#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class KoopaCapPlayerBinder;

class KoopaCap : public al::LiveActor {
public:
    static KoopaCap* create(const al::LiveActor*, const al::ActorInitInfo&, KoopaCapPlayerBinder*,
                            bool);

    KoopaCap(const char*, const al::LiveActor*);

    void init(const al::ActorInitInfo&) override;
    void makeActorDead() override;
    void appear() override;
    void kill() override;
    void disappear();
    void control() override;
    void updateCollider() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool isEquip() const;
    bool isWaitHover() const;
    bool isDown() const;
    bool isEndWaitHoverStart() const;
    bool isSpinMove() const;
    bool isAttach() const;
    bool isNecessaryUpdateAttachQT() const;
    bool isFlyBack() const;
    bool isEndFlyBack() const;
    bool isDownRecover() const;
    bool isPlayingCatchDemo() const;
    bool isPlayerBinding() const;
    void startDemo();
    void endDemo();
    void startAttach(const char*);
    void startWaitHover(s32);
    void startSpinThrowChase(const sead::Vector3f*, f32, bool);
    void startDownRecover();
    void forceStartFlyBackIfNearPlayerToKoopa();
    void requestStartAction(const char*);
    void onFinish();
    void offFinish();
    void setFastPunchRate(f32);
    void endEquipAndBlowDown();
    void endEquipAndKill();
    void exeDemo();
    void exeAppear();
    void exeWait();
    void exeAttach();
    void exeWaitHoverDelay();
    void exeWaitHoverStart();
    void exeWaitHover();
    void exeSpinThrow();
    void exeFlyBack();
    void exeFlyBackAfter();
    void exeSpinHit();
    void exeDownJumpStart();
    void exeDownJump();
    void exeDownJumpEnd();
    void exeDown();
    void exeDownRecoverStart();
    void exeDownRecover();
    void exeDownRecoverEnd();
    void exeEquip();

private:
    const al::LiveActor* mOwner = nullptr;
    u8 _110[0x60];
};

static_assert(sizeof(KoopaCap) == 0x170);
