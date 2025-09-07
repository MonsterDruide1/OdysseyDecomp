#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

class HipDropSwitch : public al::LiveActor {
public:
    HipDropSwitch(const char*);
    void init(const al::ActorInitInfo& info) override;
    void reset();
    void control() override;
    bool isOn() const;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startClipped() override;
    bool isOnWait() const;
    void writeSave();
    void validateSwitchByCage();
    void invalidateSwitchByCage();
    void exeOffWait();
    void exeReactionLand();
    void exeOn();
    void exeOnWait();
    void exeDisable();

private:
    void* _108;
    s32 _110;
    s32 _114;
    bool _118;
    s32 _11c;
    s32 _120;
    void* _128;
};

static_assert(sizeof(HipDropSwitch) == 0x130);
