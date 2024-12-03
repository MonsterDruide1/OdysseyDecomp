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
    void setConveyerKeyKeeper(const ConveyerKeyKeeper* conveyerKeyKeeper, f32 coord);
    void setTransByCoord(f32 coord, bool isMoving);
    __attribute__((noinline)) void setTransByCoord(f32 coord, bool isMoving, bool forceReset);
    void setTransAndResetByCoord(f32 coord);

    void exeWait();

private:
    LiveActor* mHost = nullptr;
    const ConveyerKeyKeeper* mConveyerKeyKeeper = nullptr;
    const char* mKeyHitReactionName = nullptr;
    const char* mActionName = nullptr;
    f32 mRelativeCoord = 0.0f;
    f32 mCoord = 0.0f;
    bool mIsExist = true;
};
}  // namespace al
