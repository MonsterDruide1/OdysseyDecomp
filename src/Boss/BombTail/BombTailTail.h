#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/Joint/JointControllerBase.h"
#include "Library/LiveActor/LiveActor.h"

class BombTailBomb;

class BombTailTailJointController : public al::JointControllerBase {
public:
    BombTailTailJointController(const sead::Matrix34f* matrix, const bool* is_control, f32 rate);
    void calcJointCallback(s32 joint_index, sead::Matrix34f* matrix) override;
    const char* getCtrlTypeName() const override;

private:
    char filler[0x48];
};

static_assert(sizeof(BombTailTailJointController) == 0x70);

class BombTailTailJointCalulator {
public:
    BombTailTailJointCalulator(al::LiveActor* actor, const al::LiveActor* parent_actor);
    void calcTailBombThrow(const sead::Matrix34f* matrix, s32 index, f32 rate);
    void calcTailBombThrowOld(const sead::Matrix34f* matrix);
    void calcBounce(const sead::Matrix34f* matrix, float value);
    void calcReturn();
    void calcBombFollowMtx(sead::Matrix34f* matrix, f32 rate);
    void validateControl();
    void controlTailVisAnim();
    void resetSampleFromAnim();

private:
    char filler[0x58];
};

static_assert(sizeof(BombTailTailJointCalulator) == 0x58);

class BombTailTail : public al::LiveActor {
public:
    BombTailTail();
    void initTail(const al::ActorInitInfo& info, al::LiveActor* parent_actor, const char* suffix,
                  s32 index);
    void calcAnim() override;
    void updateDepthShadowMap();
    void endClipped() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startBattle();
    void endBattle();
    void reset();
    void startThrow(BombTailBomb* bomb);
    void endThrow(bool is_attack_end);
    bool tryStartBounce();
    void updateBounce(const sead::Matrix34f& matrix, f32 rate);
    void hitBounce();
    void startResetAttack();
    void onAnimDynamics();
    void offAnimDynamics();
    const sead::Matrix34f* getControlTopJointMtx() const;
    f32 calcTailLength() const;
    void exeBeforeBattleStart();
    void exeFollow();
    void exeThrow();
    void exeReturn();
    void exeBounce();
    void exeReflect();

private:
    char filler[0xc8];
};

static_assert(sizeof(BombTailTail) == 0x1d0);
