#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;

class EffectObjInterval : public LiveActor {
public:
    EffectObjInterval(const char* name);

    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void appear() override;
    void kill() override;
    const sead::Matrix34f* getBaseMtx() const override;

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
    MtxConnector* mMtxConnector = nullptr;
    bool mIsStarted = false;
    s32 mEmitInterval = 0;
    s32 mTimer = 0;
    s32 mFirstEmitFrame = 0;
};

static_assert(sizeof(EffectObjInterval) == 0x150);
}  // namespace al
