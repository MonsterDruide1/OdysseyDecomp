#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
class LiveActor;
}  // namespace al

class StateMapPartsForceReset : public al::ActorStateBase {
public:
    StateMapPartsForceReset(al::LiveActor* actor, const al::ActorInitInfo& initInfo);

    void appear() override;
    bool isEnableForceReset() const;
    void exeReset();
    void exeResetEnd();

private:
    f32 mEnableResetDist;
    al::AreaObjGroup* mLinkResetAreaGroup;
    sead::Vector3f mInitTrans;
    sead::Quatf mInitQuat;
    bool mIsInvalidClipping;
};

static_assert(sizeof(StateMapPartsForceReset) == 0x50);
