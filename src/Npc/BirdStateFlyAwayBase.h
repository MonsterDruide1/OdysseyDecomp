#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
}

class BirdStateFlyAwayBase : public al::ActorStateBase {
public:
    BirdStateFlyAwayBase(const char* name, al::LiveActor* actor);

    virtual void initByPlacementInfo(const al::ActorInitInfo& info);
    void kill() override;

    void startFlyAwayByHomeLanding();
    void startFlyAwayByCapGround();
    void startFlyAwayByTouchAir();
    virtual void startFlyAwayByKoopa();

    bool isFlyAwayByHomeLanding() const;
    bool isFlyAwayByCapGround() const;
    bool isFlyAwayByTouchAir() const;
    bool isFlyAwayByKoopa() const;

protected:
    bool mHasTargetPos = false;
    sead::Vector3f mTargetPos = {0, 0, 0};

private:
    enum class FlyAwayCause { None, HomeLanding, CapGround, TouchAir, Koopa };

    FlyAwayCause mCause = FlyAwayCause::None;
};

static_assert(sizeof(BirdStateFlyAwayBase) == 0x38);
