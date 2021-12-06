#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>
#include "al/LiveActor/LiveActor.h"
#include "al/util/NerveUtil.h"

class MofumofuWarpHole : public al::LiveActor {
public:
    MofumofuWarpHole(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void disappear();
    void close();
    void closeAndDisappear();
    void open();
    void startHideMove();
    void startDashSign();
    bool isWait() const;
    bool isHideWait() const;
    void calcDashSignFront(sead::Vector3f*) const;
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeClose();
    void exeHideWait();
    void exeHideMove();
    void exeDashSign();
    void exeDashSignEnd();

private:
    sead::Quatf gap = sead::Quatf::unit;
};

namespace {

NERVE_HEADER(MofumofuWarpHole, Close)
NERVE_HEADER(MofumofuWarpHole, Disappear)
NERVE_HEADER(MofumofuWarpHole, Appear)
NERVE_HEADER(MofumofuWarpHole, HideMove)
NERVE_HEADER(MofumofuWarpHole, HideWait)
NERVE_HEADER(MofumofuWarpHole, Wait)
NERVE_HEADER(MofumofuWarpHole, DashSign)
NERVE_HEADER(MofumofuWarpHole, DashSignEnd)
NERVE_HEADER(MofumofuWarpHole, CloseAndDisappear)

}  // namespace
