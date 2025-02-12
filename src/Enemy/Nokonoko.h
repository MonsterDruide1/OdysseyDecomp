#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class WaterSurfaceFinder;
}
class CapTargetInfo;
class EnemyStateSwoon;
class IUsePlayerHack;

class Nokonoko : public al::LiveActor {
public:
    enum class MoveType : s32 { Loop = 0, Turn = 1 };

    Nokonoko(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void attackSensor(al::HitSensor* other, al::HitSensor* self) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* self,
                    al::HitSensor* other) override;

    void endCapture();

    inline bool updateAccelStick();

    void exeWait();
    void exeSwoon();
    void exeCaptureStart();
    void exeCaptureStartEnd();
    void exeCaptureEnd();
    void exeCaptureWait();
    void exeCaptureWalk();
    void exeCaptureJumpStart();
    void exeCaptureJump();
    void exeCaptureJumpEnd();
    void exeCaptureSpinStandbyStart();
    void exeCaptureSpin();
    void exeCaptureSpinEnd();
    void exeDie();

private:
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyStateSwoon* mEnemyStateSwoon = nullptr;
    IUsePlayerHack* mHackActor = nullptr;
    sead::Vector3f field_120 = sead::Vector3f::zero;
    sead::Vector3f field_12C = sead::Vector3f::zero;
    s32 mHackSwingCooldown = 0;
    MoveType mMoveType = MoveType::Loop;
    sead::Vector3f mSideDir = sead::Vector3f::ex;
    sead::Vector3f mFrontDir = sead::Vector3f::ez;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    s32 mWaterSurfaceSpringCooldown = 0;
};
