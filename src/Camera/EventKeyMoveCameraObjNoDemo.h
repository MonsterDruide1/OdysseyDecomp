#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class KeyMoveCameraObj;
}  // namespace al

class EventKeyMoveCameraObjNoDemo : public al::LiveActor {
public:
    EventKeyMoveCameraObjNoDemo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void kill() override;
    void control() override;

protected:
    struct WithDemoCtorTag {};

    EventKeyMoveCameraObjNoDemo(const char* name, WithDemoCtorTag) : al::LiveActor(name) {}

    al::KeyMoveCameraObj* mKeyMoveCameraObj = nullptr;
    s32 mStep = 0;
    bool mIsPlayed = false;
    bool mIsPlayManyTimes = false;
};

static_assert(sizeof(EventKeyMoveCameraObjNoDemo) == 0x118);
