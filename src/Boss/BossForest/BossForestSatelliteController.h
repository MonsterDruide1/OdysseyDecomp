#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
struct ActorParamF32;
struct ActorParamS32;
}  // namespace al

class BossForestSatellite;

struct BossForestSatelliteControllerParam {
    al::ActorParamF32* rotSpeed;
    al::ActorParamS32* waitTime;
    al::ActorParamS32* moveTime;
};

class BossForestSatelliteController : public al::LiveActor {
public:
    BossForestSatelliteController(s32 phaseIndex);

    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    bool isDead() const;
    bool checkCoreBroken(al::LiveActor** outBrokenCore) const;
    bool isWait() const;
    bool isAppeared() const;
    void doMove();
    void doWait();
    s32 countActiveSatellitesNum() const;
    void updateSatellitesPos(f32 angle);

    void exeAppear();
    void exeWait();
    void exeMove();

    void addSatellite(BossForestSatellite* satellite) { mSatellites.pushBack(satellite); }

    f32 getRotAngle() const { return mRotAngle; }

private:
    sead::FixedPtrArray<BossForestSatellite, 3> mSatellites;
    f32 mRotAngle = 0;
    BossForestSatelliteControllerParam* mParams = nullptr;
    s32 mPhaseIndex = 0;
    bool mIsFirstPhase = false;
};

static_assert(sizeof(BossForestSatelliteController) == 0x148);
