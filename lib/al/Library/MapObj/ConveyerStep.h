#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ConveyerKeyKeeper;

class ConveyerStep : public LiveActor {
public:
    ConveyerStep(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void setHost(LiveActor* host);
    void setConveyerKeyKeeper(const ConveyerKeyKeeper* conveyerKeyKeeper, f32);
    void setTransByCoord(f32, bool);
    void setTransByCoord(f32, bool, bool) __attribute__((noinline));
    void setTransAndResetByCoord(f32);

    void exeWait();

private:
    LiveActor* mHost = nullptr;
    const ConveyerKeyKeeper* mConveyerKeyKeeper = nullptr;
    const char* mKeyHitReactionName = nullptr;
    const char* mActionName = nullptr;
    f32 _128 = 0.0f;
    f32 _12c = 0.0f;
    bool _130 = true;
};
}  // namespace al
