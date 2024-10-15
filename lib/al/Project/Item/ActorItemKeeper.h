#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorInitInfo;
class HitSensor;

class ActorItemKeeper {
public:
    ActorItemKeeper(const LiveActor*, s32);

    void addItem(const ActorInitInfo&, const char*, const char*, const char*, s32, bool);
    void reset();
    void setFactor(const char*, const HitSensor*);
    void getAppearItemInfo(const char*); // dunno what this returns

    void* unknown[0x7];
};
} // namespace al