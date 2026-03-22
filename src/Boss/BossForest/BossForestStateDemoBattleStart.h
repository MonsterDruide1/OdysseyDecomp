#pragma once

#include <container/seadPtrArray.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class LiveActor;
}  // namespace al

class BossForest;

class BossForestStateDemoBattleStart : public al::HostStateBase<BossForest> {
public:
    BossForestStateDemoBattleStart(BossForest* boss, const al::ActorInitInfo& initInfo,
                                   al::CameraTicket* cameraTicket);

    void appear() override;
    void kill() override;

    void skipDemo();

    void exeRequestStartDemo();
    void exeDemo();
    void exeEndDemo();

private:
    al::CameraTicket* mCameraTicket = nullptr;
    sead::PtrArray<al::LiveActor> mCoreActors;
    sead::Vector3f _38 = sead::Vector3f::zero;
    sead::Quatf _44 = sead::Quatf::unit;
    sead::Vector3f _54 = sead::Vector3f::zero;
    sead::Quatf _60 = sead::Quatf::unit;
    bool _70 = false;
    u8 _71[7];
    void* _78 = nullptr;
    bool _80 = false;
};

static_assert(sizeof(BossForestStateDemoBattleStart) == 0x88);
