#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class LiveActor;
}  // namespace al

class BossForest;
class Shine;

class BossForestStateDemoBattleEnd : public al::HostStateBase<BossForest> {
public:
    BossForestStateDemoBattleEnd(BossForest* boss, const al::ActorInitInfo& initInfo,
                                 al::CameraTicket* cameraTicket);

    void appear() override;
    void kill() override;

    void exeRequestStartDemo();
    void exeDemo();
    void exeEndDemo();

private:
    Shine* mShine = nullptr;
    al::LiveActor* mPartsActor = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mAfterPlayerPos = sead::Vector3f::zero;
    sead::Quatf mAfterPlayerQuat = sead::Quatf::unit;
};

static_assert(sizeof(BossForestStateDemoBattleEnd) == 0x70);
