#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class MeganePartsState : public al::ActorStateBase {
public:
    MeganePartsState(al::LiveActor* actor);

    void appear() override;
    bool update() override;

    void exeHide();
    void exeOn();
    void updateAlpha();
    void exeShow();
    void exeOff();

private:
    f32 mAlpha = 0.0f;
};

static_assert(sizeof(MeganePartsState) == 0x28);
