#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EffectObjFollowCameraLimit : public LiveActor {
public:
    EffectObjFollowCameraLimit(const char* name);

    void init(const ActorInitInfo& info) override;
    void startAppear();
    void startDisappear();
    void control() override;
    void kill() override;
    void appear() override;
    void exeWait();
    void exeDisappear();

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
    f32 mLimitBottom = 0;
    f32 mLimitTop = -1;
};

static_assert(sizeof(EffectObjFollowCameraLimit) == 0x140);
}  // namespace al
