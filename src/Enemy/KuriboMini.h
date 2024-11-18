#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionPartsFilterBase;
class EnemyStateBlowDown;
}  // namespace al
class CollisionMultiShape;
class CollisionShapeKeeper;
class EnemyStateReset;
class EnemyStateWander;
class ActorStateSandGeyser;
class PlayerPushReceiver;

class KuriboMini : public al::LiveActor {
public:
    KuriboMini(const char*);
    void init(const al::ActorInitInfo&) override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void appear() override;
    void kill() override;
    void control() override;
    void updateCollider() override;
    void startClipped() override;
    void endClipped() override;

    virtual void appearPop();
    virtual void appearPopBack();

    bool checkSandSinkPrecisely();
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
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    void prepareKillByShineGet();
    bool tryReceiveMsgNormal(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void notifyJumpSink(f32);
    void validateSpecialPush(u32);
    void forceStartClipped();

private:
    EnemyStateReset* mStateReset;
    EnemyStateWander* mStateWander;
    ActorStateSandGeyser* mStateSandGeyser;
    al::EnemyStateBlowDown* mStateBlowDown;
    int mCounterChase;
    int _12c;
    al::CollisionPartsFilterBase* mCollisionFilter;
    void* _138[1];
    int _140;
    float mClippingRadius;
    PlayerPushReceiver* mPlayerPushReceiver;
    CollisionMultiShape* mCollisionMultiShape;
    CollisionShapeKeeper* mCollisionShapeKeeper;
    float mJumpSink;
    int mFramesSpecialPush;
    int mShiftTypeOnGround;
    al::LiveActor* _170;
    sead::Vector3f _178;
    bool mIsPopBack;
    bool mIsInvalidateClipping;
    sead::Matrix34f mColliderMtx;
};
