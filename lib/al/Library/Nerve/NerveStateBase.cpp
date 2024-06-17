#include "Library/Nerve/NerveStateBase.h"

namespace al {
NerveStateBase::NerveStateBase(const char* stateName) : NerveExecutor(stateName) {}

NerveStateBase::~NerveStateBase() = default;

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

template <class T>
HostStateBase<T>::HostStateBase(const char* name, T* host) : NerveStateBase(name), mHost(host){};

}  // namespace al
