#pragma once

#include "Camera/EventKeyMoveCameraObjNoDemo.h"

class EventKeyMoveCameraObjWithDemo : public EventKeyMoveCameraObjNoDemo {
public:
    EventKeyMoveCameraObjWithDemo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void kill() override;

private:
    const char* mDemoName = nullptr;
};

static_assert(sizeof(EventKeyMoveCameraObjWithDemo) == 0x120);
