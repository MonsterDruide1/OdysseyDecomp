#pragma once

#include "Library/Nerve/NerveStateBase.h"

class BirdStateWaitBase : public al::ActorStateBase {
public:
    using al::ActorStateBase::ActorStateBase;

    virtual const char* getDefaultWaitActionName() = 0;
};

static_assert(sizeof(BirdStateWaitBase) == 0x20);
