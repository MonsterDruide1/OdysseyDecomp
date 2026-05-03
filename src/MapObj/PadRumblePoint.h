#pragma once

#include "Library/LiveActor/LiveActor.h"

class PadRumblePoint : public al::LiveActor {
public:
    PadRumblePoint(const char* actorName);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();

private:
    const char* mRumbleName = nullptr;
    f32 mRumbleNear = 500.0f;
    f32 mRumbleFar = 2000.0f;
};

static_assert(sizeof(PadRumblePoint) == 0x118);
