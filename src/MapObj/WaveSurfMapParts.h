#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class WaveSurfMapParts : public al::LiveActor {
public:
    WaveSurfMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    bool isExistSurface() const;
    void updateFinder();
    void approachSurface();
    void syncSurfaceUp();
    void exeWait();
    void exeSink();
    void exeSinkDeep();
    f32 getSurfaceHeight() const;

private:
    u8 _108[0x40];
    bool _148;
    u8 _149[3];
    s32 _14c;
    void* _150;
};

static_assert(sizeof(WaveSurfMapParts) == 0x158);
