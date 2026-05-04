#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class Shine;

namespace al {
class LiveActor;
}

class EventActorStateGiveShine : public al::ActorStateBase {
public:
    EventActorStateGiveShine(al::LiveActor* actor);

    void start(Shine* shine, const sead::Vector3f& trans, const char* actionName);
    void exeStart();
    void exeWait();

private:
    Shine* mShine = nullptr;
    sead::Vector3f mShineAppearTrans = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(EventActorStateGiveShine) == 0x38);
