#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
class CollisionPartsFilterBase;
}  // namespace al
class ActorStateSandGeyser;
class EnemyStateReset;
class EnemyStateWander;
class PlayerPushReceiver;
class CollisionMultiShape;
class CollisionShapeKeeper;

class KuriboMini : public al::LiveActor {
public:
    KuriboMini(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void appear() override;
    void kill() override;
    virtual void appearPop();
    virtual void appearPopBack();
    void control() override;
    bool checkSandSinkPrecisely();
    void updateCollider() override;
    void startClipped() override;
    void endClipped() override;
    void noRevive();
    void exeWait();
    void setShiftTypeOnGround(s32);
    bool isPlayerUp();
    void exeWander();
    bool tryShiftDrown();
    void exeTurn();
    void exeFind();
    void exeChaseReady();
    void exeChase();
    void exeStop();
    void exeAttack();
    void exeBlow();
    void exeBlowLand();
    void exeBlowRecover();
    void exePressDown();
    void exeBlowDown();
    void exeFall();
    void exeLand();
    bool tryShiftChaseOrWander();
    void exeSink();
    bool updateSink();
    void exeReset();
    void exeSandGeyser();
    void exeDrown();
    void exeHide();
    void exePopAppearStart();
    void exePopAppear();
    void exePopAppearEnd();
    void exeBind();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void prepareKillByShineGet();
    bool tryReceiveMsgNormal(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self);
    void notifyJumpSink(f32);
    void validateSpecialPush(u32);
    void forceStartClipped();

private:
    EnemyStateReset* mStateReset;
    EnemyStateWander* mStateWander;
    ActorStateSandGeyser* mStateSandGeyser;
    al::EnemyStateBlowDown* mStateBlowDown;
    s32 mCounterChase;
    s32 _12c;
    al::CollisionPartsFilterBase* mCollisionFilter;
    void* _138[1];
    s32 _140;
    f32 mClippingRadius;
    PlayerPushReceiver* mPlayerPushReceiver;
    CollisionMultiShape* mCollisionMultiShape;
    CollisionShapeKeeper* mCollisionShapeKeeper;
    f32 mJumpSink;
    s32 mFramesSpecialPush;
    s32 mShiftTypeOnGround;
    al::LiveActor* _170;
    sead::Vector3f _178;
    bool mIsPopBack;
    bool mIsInvalidateClipping;
    sead::Matrix34f mColliderMtx;
};
