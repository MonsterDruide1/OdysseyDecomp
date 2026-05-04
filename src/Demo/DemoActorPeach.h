#pragma once

#include "Library/Demo/DemoActor.h"

class DemoActorPeach : public al::DemoActor {
public:
    DemoActorPeach(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void makeActorAlive() override;

private:
    bool mIsUseCapManHeroineEyes = true;
};

static_assert(sizeof(DemoActorPeach) == 0x170);
