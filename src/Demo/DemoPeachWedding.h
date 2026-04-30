#pragma once

#include "Library/Demo/DemoActor.h"

class DemoPeachWedding : public al::DemoActor {
public:
    DemoPeachWedding(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;

private:
    bool mIsUseCapManHeroineEyes = false;
};

static_assert(sizeof(DemoPeachWedding) == 0x170);
