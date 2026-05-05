#pragma once

#include "Library/Nerve/NerveStateBase.h"

class IUsePlayerHack;

namespace al {
class LiveActor;
}  // namespace al

class HackerStateBase : public al::NerveStateBase {
public:
    HackerStateBase(const char* name, al::LiveActor* actor, IUsePlayerHack** hacker);

protected:
    al::LiveActor* mActor;
    IUsePlayerHack** mHacker;
};

static_assert(sizeof(HackerStateBase) == 0x28);
