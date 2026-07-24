#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class GrowSeedStateRevival : public al::ActorStateBase {
public:
    GrowSeedStateRevival(const char* name, al::LiveActor* actor, sead::Quatf* quat);

    void appear() override;
    void kill() override;

    void exeRevivalWait();
    void exeRevival();

private:
    sead::Vector3f mRevivalTrans = sead::Vector3f::zero;
    sead::Quatf* mRevivalQuat;
};

static_assert(sizeof(GrowSeedStateRevival) == 0x38);
