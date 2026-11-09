#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

class EnemyCap;
class FrogParam;
class IUsePlayerHack;

namespace al {
class JointRippleGenerator;
class JointSpringControllerHolder;
class WaterSurfaceFinder;
}  // namespace al

class Frog : public al::LiveActor {
public:
    Frog(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void updateCollider() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void die();
    void updateWaterSurface();
    bool isOnWaterSurface() const;
    // True when any swim animation is playing. Name is a guess.
    bool isSwimActionPlaying() const;
    void updateVelocity();
    void updateSurfaceTransform();
    void surfaceJump();
    void startHack(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void onDynamics();
    void offDynamics();
    void endHack();

    void exeAppear();
    void exeWait();
    void exeWander();
    void exeDamageCap();
    void exeTrampled();
    void exeWaitHackStart();
    void exeWaitHackStartSwoon();
    void exeDemoHackFirst();
    void exeHackStart();
    void exeSwoon();
    void exeHack();
    void exeRevive();
    void exeReviveAppear();
    void exeReset();
    void exeDie();

private:
    FrogParam* mFrogParam = nullptr;                                    // 0x108
    IUsePlayerHack* mPlayerHack = nullptr;                              // 0x110
    u8 _118[0x28];                                                      // 0x118-0x13f
    EnemyCap* mEnemyCap = nullptr;                                      // 0x140
    u8 _148[0x8];                                                       // 0x148-0x14f
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;              // 0x150
    u8 _158[0x8];                                                       // 0x158-0x15f
    sead::Matrix34f mSurfaceMtx;                                        // 0x160-0x18f
    s32 mSurfaceJumpCount = 0;                                          // 0x190
    u8 _194[0x14];                                                      // 0x194-0x1a7
    void* _1a8 = nullptr;                                               // 0x1a8
    al::JointRippleGenerator* mJointRippleGenerator1 = nullptr;         // 0x1b0
    al::JointRippleGenerator* mJointRippleGenerator2 = nullptr;         // 0x1b8
    u8 _1c0[0x8];                                                       // 0x1c0-0x1c7
    void* _1c8 = nullptr;                                               // 0x1c8
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;  // 0x1d0
    bool mIsRevive = false;  // 0x1d8, name unverified: true -> Revive, false -> Reset
    bool _1d9 = false;                                                  // 0x1d9
};
