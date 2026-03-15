#pragma once

#include "Library/LiveActor/LiveActor.h"

class GabuZouGroup;

class GabuZou : public al::LiveActor {
public:
    GabuZou(const char* name);
    void init(const al::ActorInitInfo& info) override;

    f32 calcSerchSizeX() const;
    f32 calcSerchSizeZ() const;
    void initAfterPlacement() override;
    bool checkCollision(const sead::Vector3i&) const;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void setGabuzouGroup(const GabuZouGroup*);
    bool checkReach(s32, s32) const;
    bool checkReach(const sead::Vector3f&) const;
    bool isInSerchRange(const sead::Vector3i&) const;
    f32 calcPlayerExpectPos() const;
    bool isLeftPlayer() const;
    void nextNerve();
    void exeWait();
    void exeMove();
    void exeTurn();
    void exeRiseSign();
    void exeRise();
    void exeAttackSign();
    void exeAttack();
    void exeSink();

private:
    char _108[0x1D0 - sizeof(al::LiveActor)];
};
