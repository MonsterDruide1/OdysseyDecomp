#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveUtil.h"

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
