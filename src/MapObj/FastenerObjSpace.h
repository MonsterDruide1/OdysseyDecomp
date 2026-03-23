#pragma once

#include "Library/LiveActor/LiveActor.h"

class FastenerObjSpace : public al::LiveActor {
public:
    FastenerObjSpace(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void disappear();
    void exeWait();
    void exeDisappear();

private:
    al::LiveActor* mLinkActor = nullptr;
};

static_assert(sizeof(FastenerObjSpace) == 0x110);
