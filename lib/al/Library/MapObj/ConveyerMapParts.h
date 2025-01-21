#pragma once

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
    void start();
    void stop();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;
    void startClipped() override;
    void endClipped() override;

    void exeStandBy();
    void exeMove();

private:
    DeriveActorGroup<ConveyerStep>* mConveyerSteps = nullptr;
    ConveyerKeyKeeper* mConveyerKeyKeeper = nullptr;
    sead::Vector3f _118 = sead::Vector3f::zero;
    f32 _124 = 0.0f;
    f32 mMoveSpeed = 5.0f;
    f32 mPartsInterval = 200.0f;
    f32 _130 = 0.0f;
    s32 _134 = 0;
    s32 _138 = 0;
    s32 _13c = 30;
    bool mIsRideOnlyMove = false;
};

static_assert(sizeof(ConveyerMapParts) == 0x148);
}  // namespace al
