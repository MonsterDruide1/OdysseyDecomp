#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;

enum class FloaterMoveType : s32 { Wait, Back, Sink };

class FloaterMapParts : public LiveActor {
public:
    FloaterMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
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
    FloaterMoveType mMoveType = FloaterMoveType::Wait;
    s32 mSinkTime = 0;
    s32 mSinkKeepTime = 10;
    s32 mMaxAccelCount = 10;
    s32 mAccelCount = 0;
    sead::Matrix34f mMtxTR = sead::Matrix34f::ident;
};
}  // namespace al
