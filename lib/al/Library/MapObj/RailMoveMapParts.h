#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;

class RailMoveMapParts : public LiveActor {
public:
    RailMoveMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    void stop();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;
    void appearAndSetStart();

    void exeStandBy();
    void exeMoveSign();
    void exeMove();

private:
    SwitchKeepOnAreaGroup* mSwitchKeepOnAreaGroup = nullptr;
    SwitchOnAreaGroup* mSwitchOnAreaGroup = nullptr;
    sead::Vector3f mRailPos = {0.0f, 0.0f, 0.0f};
    f32 mRailCoord = 0.0f;
};

static_assert(sizeof(RailMoveMapParts) == 0x128);
}  // namespace al
