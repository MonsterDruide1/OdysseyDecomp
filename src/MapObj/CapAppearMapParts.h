#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class CapAppearMapParts : public al::LiveActor {
public:
    CapAppearMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void listenAppear();
    void startWait();
    void killAll();
    void control() override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeAppear();
    void exeAppearEnd();
    void exeWait();
    void exeReaction();
    void exeWaitForever();
    void exeDisappear();

private:
    void* _108 = nullptr;
    s32 _110 = 0;
    sead::Vector3f _114 = sead::Vector3f::zero;
    s32 _120 = 0;
    bool mIs124 = false;
    sead::Vector3f _128 = sead::Vector3f::zero;
    bool mIs134 = false;
};

static_assert(sizeof(CapAppearMapParts) == 0x138);
