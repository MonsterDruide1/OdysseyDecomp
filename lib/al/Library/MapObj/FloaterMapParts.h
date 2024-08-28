#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Movement/MoveType.h"

namespace al {
class KeyPoseKeeper;

class FloaterMapParts : public LiveActor {
public:
    FloaterMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;
    void appearAndSetStart();
    void control() override;
    void exeWait();
    void exeSink();
    void exeBack();

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    f32 mCoord = 0.0f;
    f32 mMaxCoord = 0.0f;
    f32 mSinkSpeed = 5.0f;
    f32 mBackSpeed = 5.0f;
    MoveType mMoveType = MoveType::Loop;
    s32 mSinkTime = 0;
    s32 mSinkKeepTime = 10;
    s32 mMaxAccelCount = 10;
    s32 mAccelCount = 0;
    sead::Matrix34f mMtxTR = sead::Matrix34f::ident;
};
}  // namespace al
