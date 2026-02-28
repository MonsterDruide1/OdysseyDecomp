// TODO
#pragma once

#include "Library/LiveActor/LiveActor.h"

class BrosEnemyMovePositionUpdater {
public:
    BrosEnemyMovePositionUpdater(const al::ActorInitInfo&, al::LiveActor*);

    void initAfterPlacement();
    void update();
    void reset();
    void nextMovePoint();
    void getNextTrans(sead::Vector3f*) const;
    void isUseConnector() const;

private:
    char _0[0x38];
};

class BrosWeaponBase : public al::LiveActor {
public:
    BrosWeaponBase(const char* name, const al::LiveActor* bros);

    void attach(const sead::Matrix34<float>*, const sead::Vector3f&, const sead::Vector3f&,
                const char*);
    void calcAttachMtx(sead::Matrix34<float>*, const sead::Matrix34<float>*, const sead::Vector3f&,
                       const sead::Vector3f&);

    virtual u32 getBreakStep() const { return 0; }

    virtual bool isBreak() const { return false; }
};

class BrosHackBase : public al::LiveActor {
public:
    BrosHackBase(s32, const char*, bool, const char*);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void attackFront(al::HitSensor*, al::HitSensor*);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void setupReset();
    void endClipped();
    void kill();
    void tryAttachMtxConnect();
    void tryDetachMtxConnect();
    void tryConnectMtx();
    void tryConnectResetMtx();
    void tryBlowDownIfAlive(const sead::Vector3f&, al::HitSensor*);
    void tryAttachWeapon();
    void tryFindDeadOrBreakWeapon() const;
    void isEnableAttackWeapon(bool) const;
    void tryAttachAttackWeapon();
    void killShineGetDemo();
    void tryKillAttachWeapon();
    void isHacked() const;
    void exeEnemyMovement();
    void exeHackMovement();
    void exeReset();
    void exeRevive();

    virtual void attach() {}

    virtual void attachAttack() {}
};
