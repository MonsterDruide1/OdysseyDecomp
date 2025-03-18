#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;

class EffectObjCameraEmit : public LiveActor {
public:
    EffectObjCameraEmit(const char* name);

    void init(const ActorInitInfo& info) override;
    void switchOnKill();
    void initAfterPlacement() override;
    void control() override;
    bool tryEmitStart();
    void appear() override;
    void kill() override;
    const sead::Matrix34f* getBaseMtx() const override;

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
    MtxConnector* mMtxConnector = nullptr;
    bool mIsStarted = false;
    bool mIsStopFollowCamera = false;
};

static_assert(sizeof(EffectObjCameraEmit) == 0x148);
}  // namespace al
