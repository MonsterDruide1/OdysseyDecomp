#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActorKit.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;

class ItemDirectorBase {
public:
    virtual void appearItem(const char*, const sead::Vector3f&, const sead::Quatf&,
                            const HitSensor*) = 0;
    virtual void acquireItem(const LiveActor*, HitSensor*, const char*) = 0;
    virtual void declareItem(const char*, const ActorInitInfo&) = 0;
    virtual void endInit() = 0;
};

}  // namespace al
