#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerStateWait : public al::ActorStateBase{
public:
    bool isEnableCancelAction() const;

private:
    u8 padding[0xE8 - sizeof(al::ActorStateBase)]; 
};
