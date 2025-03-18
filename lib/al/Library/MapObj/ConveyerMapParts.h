#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
template <class T>
class DeriveActorGroup;
class ConveyerStep;
class ConveyerKeyKeeper;

class ConveyerMapParts : public LiveActor {
public:
    ConveyerMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;
    void startClipped() override;
    void endClipped() override;

    void start();
    void stop();

    void exeStandBy();
    void exeMove();

private:
    DeriveActorGroup<ConveyerStep>* mConveyerStepGroup = nullptr;
    ConveyerKeyKeeper* mConveyerKeyKeeper = nullptr;
    sead::Vector3f mClippingTrans = sead::Vector3f::zero;
    f32 mOffsetCoord = 0.0f;
    f32 mMoveSpeed = 5.0f;
    f32 mPartsInterval = 200.0f;
    f32 mMaxCoord = 0.0f;
    s32 mAddRideActiveFrames = 0;
    s32 mRideActiveFrames = 0;
    s32 mMaxRideActiveFrames = 30;
    bool mIsRideOnlyMove = false;
};

static_assert(sizeof(ConveyerMapParts) == 0x148);
}  // namespace al
