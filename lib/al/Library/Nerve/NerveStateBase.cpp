#include "Library/Nerve/NerveStateBase.h"

namespace al {
NerveStateBase::NerveStateBase(const char* stateName) : NerveExecutor(stateName) {}

void NerveStateBase::init() {}

void NerveStateBase::appear() {
    mIsDead = false;
}

void NerveStateBase::kill() {
    mIsDead = true;
}

bool NerveStateBase::update() {
    updateNerve();

    if (mIsDead)
        return true;

    control();

    return mIsDead;
}

void NerveStateBase::control() {}

ActorStateBase::ActorStateBase(const char* name, LiveActor* actor)
    : NerveStateBase(name), mActor(actor) {}

}  // namespace al
