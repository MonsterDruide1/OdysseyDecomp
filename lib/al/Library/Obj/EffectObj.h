#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Movement/MoveType.h"

namespace al {
class MtxConnector;

class EffectObj : public LiveActor {
public:
    EffectObj(const char* name);

    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void appear() override;
    void kill() override;
    virtual bool tryEmitStart();

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
    MtxConnector* mMtxConnector = nullptr;
    MoveType mMoveType = MoveType::Loop;
    f32 mRailMoveSpeed = 0;
    sead::Vector3f mRailPos = {0, 0, 0};
    bool mIsSyncRailPose = false;
    bool mIsStarted = false;
    bool mIsEffectObjScale = false;
};

static_assert(sizeof(EffectObj) == 0x158);
}  // namespace al
