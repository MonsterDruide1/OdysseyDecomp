#include "al/nerve/NerveStateBase.h"

namespace al {
NerveStateBase::NerveStateBase(const char* pName) : NerveExecutor(pName) {}
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

    if (mIsDead) {
        return true;
    }

    control();

    return mIsDead;
}

void NerveStateBase::control() {}

ActorStateBase::ActorStateBase(const char* name, al::LiveActor* liveActor)
    : NerveStateBase(name), mLiveActor(liveActor) {}
};  // namespace al