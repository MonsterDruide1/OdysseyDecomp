#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class LifeUpItemStateAutoGet : public al::ActorStateBase {
public:
    LifeUpItemStateAutoGet(al::LiveActor* actor, bool is2D);

    void appear() override;

    void exePopup();

private:
    bool mIs2D = false;
};

static_assert(sizeof(LifeUpItemStateAutoGet) == 0x28);
