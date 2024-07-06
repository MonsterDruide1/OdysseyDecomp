#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace al {
class EffectObjFollowCamera : public LiveActor {
public:
    EffectObjFollowCamera(const char* name);

    void init(const ActorInitInfo& info) override;
    void startAppear();
    void startDisappear();
    void control() override;
    void exeWait();
    void exeDisappear();

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(EffectObjFollowCamera) == 0x138);

namespace {
NERVE_IMPL(EffectObjFollowCamera, Wait)
NERVE_IMPL(EffectObjFollowCamera, Disappear)
}  // namespace
}  // namespace al
