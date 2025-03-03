#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AreaObjGroup;
}

class CapTargetInfo;
class IUsePlayerHack;
class PlayerHackStartShaderCtrl;

struct FukuwaraiPart {
    const char* name;
    f32 basePoints;
    f32 anglePoints;
    f32 distancePoints;
};

class FukuwaraiFaceParts : public al::LiveActor {
public:
    FukuwaraiFaceParts(const char* name, al::AreaObjGroup* group);

    void init(const al::ActorInitInfo& info) override;
    s32 getPriority() const;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    f32 calcScore(bool isMario) const;
    bool isPlaced() const;
    f32 calcScoreAngleRate() const;
    f32 calcScoreDistRate() const;
    void show();
    void reset();
    void vanish();

    void exePlace();
    void exeReset();
    void exeAppear();
    void exeWait();
    void exeCaptureWait();
    void exeCaptureMove();
    void exeHide();
    void exeCaptureStart();
    void exeCaptureHackStart();
    bool isKuriboMarioParts() const;

private:
    IUsePlayerHack* mIUsePlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    f32 mTargetAngle = 0.0f;
    sead::Vector3f mTargetPos = sead::Vector3f::zero;
    al::AreaObjGroup* mFukuwaraiArea = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mRotation = sead::Vector3f::zero;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
};
