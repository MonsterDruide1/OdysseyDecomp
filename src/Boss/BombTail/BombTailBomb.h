#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class BombTailDamage;

class BombTailBomb : public al::LiveActor {
public:
    BombTailBomb(const char* name, const al::LiveActor* parent_actor);
    void init(const al::ActorInitInfo& info) override;
    bool tryStartFollow(const char* action_name);
    void kill() override;
    void calcAnim() override;
    bool isChanceBomb() const;
    void changeAtOnGround(const sead::Vector3f* position);
    void startWaitForDemo(BombTailDamage* damage, const sead::Matrix34f* matrix);
    void startBattle();
    void setTailThrowTargetPos(const sead::Vector3f& position);
    void startAppear(BombTailDamage* damage, const sead::Matrix34f* matrix);
    void startPrepareAttackChance(BombTailDamage* damage, const sead::Matrix34f* matrix);
    void startPopUp(const sead::Vector3f& position, const sead::Vector3f& velocity,
                    BombTailDamage* damage, s32 delay);
    void startExplosion(bool is_chance);
    bool isReflect() const;
    bool isReflectChance() const;
    bool isLandChance() const;
    bool isExplosioned() const;
    void calcThrowChancePos(sead::Vector3f* position, f32 rate);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startAttackChanceSign(bool is_first, bool is_last);
    void startAttackStart(al::LiveActor* actor);
    void startThrow(f32 speed);
    void invalidateFollowForChanceReflect();
    void syncReflectChance();
    void tryChanceEnd();
    void exeBeforeBattleStart();
    void exeAppear();
    void exeAppearChance();
    void exeFollowWait();
    void exeThrowPopup();
    void exeBound();
    void exeLand();
    void exeLandChance();
    void exeThrow();
    void exeReflect();
    void exeReflectCorrect();
    void exeExplosionSign();
    void exeBlowDown();
    void exeExplosion();
    void validateFollowForChanceReflect();

private:
    char filler[0xe8];
};

static_assert(sizeof(BombTailBomb) == 0x1f0);
