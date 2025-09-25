#include "Library/Nerve/NerveStateBase.h"

namespace al {
NerveStateBase::NerveStateBase(const char* stateName) : NerveExecutor(stateName) {}

bool NerveStateBase::update() {
    updateNerve();

    if (mIsDead)
        return true;

    control();

    return mIsDead;
}

ActorStateBase::ActorStateBase(const char* name, LiveActor* actor)
    : NerveStateBase(name), mActor(actor) {}
}  // namespace al
