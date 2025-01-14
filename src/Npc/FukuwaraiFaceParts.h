#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AreaObjGroup;
}

class CapTargetInfo;
class IUsePlayerHack;
class PlayerHackStartShaderCtrl;

class FukuwaraiFaceParts : public al::LiveActor {
public:
    FukuwaraiFaceParts(const char* name, al::AreaObjGroup* group);
    void init(const al::ActorInitInfo& info) override;
    void getPriority() const;
    void control() override;
    bool receiveMsg(al::SensorMsg const* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void calcScore(bool unk) const;
    void isPlaced() const;
    void calcScoreAngleRate() const;
    void calcScoreDistRate() const;
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
    void isKuriboMarioParts() const;

private:
    IUsePlayerHack* mIUsePlayerHack;
    CapTargetInfo* mCapTargetInfo;
    f32 field_118;
    sead::Vector3f mTargetPos;
    al::AreaObjGroup* mAreaObjGroup;
    sead::Vector3f mTrans;
    sead::Vector3f mRotate;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl;
};
